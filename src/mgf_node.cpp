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

void mgf_node_model::construct_from_ainode(aiNode *ainode, mgf_data *data){
	if(ainode == NULL) return;
	//std::cerr << ainode->mName.C_Str() << std::endl;
	_data = data;
	_name = ainode->mName.C_Str();
	_num_meshes = ainode->mNumMeshes;
	_num_children = ainode->mNumChildren;

	_meshes.resize(_num_meshes);
	_child_nodes.resize(_num_children);
	_meshes.assign(ainode->mMeshes, ainode->mMeshes + _num_meshes);

	for(unsigned int i = 0; i < _num_children; i++){
		_child_nodes[i] = new mgf_node_model;
		_child_nodes[i]->_parent_node = this;
		((mgf_node_model *)_child_nodes[i])->construct_from_ainode(ainode->mChildren[i], data);
	}
	return;
}

void mgf_node_model::render(){
	return;
}


//######################  node_model_instance

}









