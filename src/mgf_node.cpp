/*
**	Author:		Martin Schwarz
**	Name:		mgf_node.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_scene.h"

namespace mgf{

//######################  node

mgf_node::mgf_node(){
	_parent_node = NULL;
	_num_children = 0;
	_num_meshes = 0;
	_num_instances = 0;
	_render = true;
}

mgf_node::~mgf_node(){
	_parent_node = NULL;
	_num_children = 0;
	_num_meshes = 0;
	_num_instances = 0;
	_render = true;
}

void mgf_node::construct_from_ainode(aiNode *ainode){
	if(ainode == NULL) return;
	//std::cerr << ainode->mName.C_Str() << std::endl;
	_name = ainode->mName.C_Str();
	_num_meshes = ainode->mNumMeshes;
	_num_children = ainode->mNumChildren;

	_meshes.resize(_num_meshes);
	_child_nodes.resize(_num_children);
	_meshes.assign(ainode->mMeshes, ainode->mMeshes + _num_meshes);

	for(unsigned int i = 0; i < _num_children; i++){
		_child_nodes[i] = new mgf_node;
		_child_nodes[i]->_parent_node = this;
		_child_nodes[i]->construct_from_ainode(ainode->mChildren[i]);
	}
	return;
}

mgf_node *mgf_node::find_node(std::string name){
	if(name == _name) return this;
	mgf_node *node;
	for(unsigned int i = 0; i < _num_children; i++){
		if((node = _child_nodes[i]->find_node(name)) != NULL) return node;
	}
	return NULL;
}

mgf_instance *mgf_node::create_instance(bool relative, bool still_renderable){
	mgf_instance *inst = new mgf_instance(this);
	_instances.push_back(inst);
	return inst;
}

void mgf_node::render(){
	return;
}

//######################  instance

mgf_instance::mgf_instance(){
	_parent = NULL;
	_relative_to_parent = true;
}

mgf_instance::mgf_instance(mgf_node *parent){
	_parent = parent;
	_relative_to_parent = true;
}

mgf_instance::~mgf_instance(){
	_parent = NULL;
}

}









