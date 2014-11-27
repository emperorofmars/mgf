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

bool mgf_node::add_child(mgf_node *node){
	if(node == NULL) return false;
	_child_nodes.push_back(node);
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

	newnode->_meshes.resize(ainode->mNumMeshes);
	for(unsigned int i = 0; i < newnode->_meshes.size(); i++)
		newnode->_meshes[i] = ainode->mMeshes[i] + oldsize_meshes;

	for(unsigned int i = 0; i < ainode->mNumChildren; i++)
		newnode->construct_from_ainode(ainode->mChildren[i], data, oldsize_meshes);

	return;
}

void mgf_node_model::render(){
	mgf::render(this, _data);
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
	return;
}

void mgf_node_model_instance::render(){
	return;
}


}



















