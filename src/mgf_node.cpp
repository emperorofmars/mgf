/*
**	Author:		Martin Schwarz
**	Name:		mgf_node.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_loader.h"

namespace mgf{

//######################  node

unsigned int mgf_node::_global_id = 0;

mgf_node::mgf_node(){
	_id = _global_id;
	_num_children = 0;
	_parent_node = NULL;
}

mgf_node::~mgf_node(){
	for(unsigned int i = 0; i < _child_nodes.size(); i++)
		delete _child_nodes[i];
	_child_nodes.clear();
	_parent_node = NULL;
}

mgf_node *mgf_node::find_node(std::string name){
	if(name == _name) return this;
	mgf_node *node;
	for(unsigned int i = 0; i < _num_children; i++){
		if((node = _child_nodes[i]->find_node(name)) != NULL) return node;
	}
	return NULL;
}

/*mgf_node *mgf_node::get_by_path(std::string path){	//old fallback version
	if(path == _name) return this;

	int pos = path.find("/", 0);
	std::string name = path.substr(0, pos);
	path = path.substr(pos + 1, path.size());

	if(_name == name) return get_by_path(path);

	for(unsigned int i = 0; i < _num_children; i++){
		if(_child_nodes[i]->_name == name){
			return _child_nodes[i]->get_by_path(path);
		}
	}
	return NULL;
}*/

mgf_node *mgf_node::get_by_path(std::string path){
	if(path == _name) return this;

	unsigned int pos = path.find("/", 0);
	int num = 1;
	std::string name = path.substr(0, pos);
	path = path.substr(pos + 1, path.size());

	if(_name == name) return get_by_path(path);

	if(name.size() > 2){
		pos = name.find("%", 0);
		if(pos != std::string::npos){
			num = atoi(name.substr(pos + 1, name.size()).c_str());
			if(num == 0) num = 1;
			name = name.substr(0, pos);
		}
	}

	for(unsigned int i = 0; i < _num_children; i++){
		if(_child_nodes[i]->_name == name){
			if(num <= 1){
				return _child_nodes[i]->get_by_path(path);
			}
			else num--;
		}
	}
	return NULL;
}

bool mgf_node::add(mgf_node *node){
	if(node == NULL) return false;
	_child_nodes.push_back(node);
	_num_children = _child_nodes.size();
	node->_parent_node = this;
	return true;
}

void mgf_node::print(unsigned int deepness){
	deepness ++;
	for(unsigned int i = 0; i < deepness; i++) std::cerr << "|_";
	std::cerr << _name << std::endl;
	for(unsigned int i = 0; i < _num_children; i++){
        _child_nodes[i]->print(deepness);
	}
	return;
}

//######################  node_model

mgf_node_model::mgf_node_model(){
	_data = NULL;
	_num_meshes = 0;
	_num_instances = 0;
	_trans = glm::mat4(1);
	_render = true;
}

mgf_node_model::~mgf_node_model(){
}

void mgf_node_model::construct_from_ainode(aiNode *ainode, mgf_data *data, unsigned int oldsize_meshes){
	if(ainode == NULL) return;
	//std::cerr << _name << " CONSTRUCT: " << ainode->mName.C_Str() << std::endl;

	mgf_node_model *newnode = new mgf_node_model;
	newnode->_parent_node = this;
	_child_nodes.push_back(newnode);

	//if(_child_nodes[0] == newnode)std::cerr << "SAME AS FIRST" << std::endl;

	_num_children = _child_nodes.size();

	newnode->_data = data;
	newnode->_name = ainode->mName.C_Str();
	newnode->_num_meshes = ainode->mNumMeshes;
	newnode->_num_children = ainode->mNumChildren;

	newnode->_trans[0][0] = ainode->mTransformation.a1; newnode->_trans[0][1] = ainode->mTransformation.b1;	//get matrix
	newnode->_trans[0][2] = ainode->mTransformation.c1; newnode->_trans[0][3] = ainode->mTransformation.d1;
	newnode->_trans[1][0] = ainode->mTransformation.a2; newnode->_trans[1][1] = ainode->mTransformation.b2;
	newnode->_trans[1][2] = ainode->mTransformation.c2; newnode->_trans[1][3] = ainode->mTransformation.d2;
	newnode->_trans[2][0] = ainode->mTransformation.a3; newnode->_trans[2][1] = ainode->mTransformation.b3;
	newnode->_trans[2][2] = ainode->mTransformation.c3; newnode->_trans[2][3] = ainode->mTransformation.d3;
	newnode->_trans[3][0] = ainode->mTransformation.a4; newnode->_trans[3][1] = ainode->mTransformation.b4;
	newnode->_trans[3][2] = ainode->mTransformation.c4; newnode->_trans[3][3] = ainode->mTransformation.d4;

	newnode->_meshes.resize(ainode->mNumMeshes);
	for(unsigned int i = 0; i < newnode->_meshes.size(); i++)
		newnode->_meshes[i] = ainode->mMeshes[i] + oldsize_meshes;

	for(unsigned int i = 0; i < ainode->mNumChildren; i++)
		newnode->construct_from_ainode(ainode->mChildren[i], data, oldsize_meshes);

	return;
}

mgf_node_model_instance *mgf_node_model::create_instance(){
	mgf_node_model_instance *node = new mgf_node_model_instance;
	node->_model_id = _id;
	node->_name = _name;
	node->_model = this;
	node->_trans = _trans;
	return node;
}

void mgf_node_model::render(){
	mgf::render(this);
	return;
}

//######################  node_model_instance

mgf_node_model_instance::mgf_node_model_instance(){
	_model_id = 0;
	_model = NULL;
	_trans = glm::mat4(1);
	_render = true;
}

mgf_node_model_instance::~mgf_node_model_instance(){
}

void mgf_node_model_instance::construct_from_mgf_node(mgf_node_model *node){
	if(node == NULL) return;

	mgf_node_model_instance *newnode = new mgf_node_model_instance;
	newnode->_parent_node = this;
	_child_nodes.push_back(newnode);

	_num_children = _child_nodes.size();

	newnode->_model = node;
	newnode->_name = node->_name;
	newnode->_model_id = node->_id;
	newnode->_trans = node->_trans;

	for(unsigned int i = 0; i < node->_num_children; i++)
		newnode->construct_from_mgf_node((mgf_node_model *)node->_child_nodes[i]);

	return;
}

void mgf_node_model_instance::render(){
	mgf::render(this);
	return;
}


}



















