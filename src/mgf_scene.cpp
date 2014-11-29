/*
**	Author:		Martin Schwarz
**	Name:		mgf_scene.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_loader.h"

namespace mgf{

//######################  scene

scene::scene(){
	_data = new mgf_data;
}

scene::~scene(){
	delete _data;
}

void scene::render(){
	for(unsigned int i = 0; i < _root_instances.size(); i++)
		mgf::render(_root_instances[i]);
	return;
}

void scene::render_repository(){
	for(unsigned int i = 0; i < _root_repository.size(); i++)
		mgf::render(_root_repository[i]);
	return;
}

//######################  scene transform

bool scene::translate(std::string name, glm::vec3 data){
	mgf_node_model_instance *node = NULL;
	for(unsigned int i = 0; i < _root_instances.size() && node == NULL; i++)
		node = (mgf_node_model_instance *)_root_instances[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::translate(glm::mat4(1), data);
	return true;
}

bool scene::rotate(std::string name, float angle, glm::vec3 data){
	mgf_node_model_instance *node = NULL;
	for(unsigned int i = 0; i < _root_instances.size() && node == NULL; i++)
		node = (mgf_node_model_instance *)_root_instances[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::rotate(glm::mat4(1), angle, data);
	return true;
}

bool scene::scale(std::string name, glm::vec3 data){
	mgf_node_model_instance *node = NULL;
	for(unsigned int i = 0; i < _root_instances.size() && node == NULL; i++)
		node = (mgf_node_model_instance *)_root_instances[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::scale(glm::mat4(1), data);
	return true;
}

bool scene::multiply_mat(std::string name, glm::mat4 data){
	mgf_node_model_instance *node = NULL;
	for(unsigned int i = 0; i < _root_instances.size() && node == NULL; i++)
		node = (mgf_node_model_instance *)_root_instances[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= data;
	return true;
}

//######################  scene transform in repo

bool scene::repo_translate(std::string name, glm::vec3 data){
	mgf_node_model *node = NULL;
	for(unsigned int i = 0; i < _root_repository.size() && node == NULL; i++)
		node = (mgf_node_model *)_root_repository[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::translate(glm::mat4(1), data);
	return true;
}

bool scene::repo_rotate(std::string name, float angle, glm::vec3 data){
	mgf_node_model *node = NULL;
	for(unsigned int i = 0; i < _root_repository.size() && node == NULL; i++)
		node = (mgf_node_model *)_root_repository[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::rotate(glm::mat4(1), angle, data);
	return true;
}

bool scene::repo_scale(std::string name, glm::vec3 data){
	mgf_node_model *node = NULL;
	for(unsigned int i = 0; i < _root_repository.size() && node == NULL; i++)
		node = (mgf_node_model *)_root_repository[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::scale(glm::mat4(1), data);
	return true;
}

bool scene::repo_multiply_mat(std::string name, glm::mat4 data){
	mgf_node_model *node = NULL;
	for(unsigned int i = 0; i < _root_repository.size() && node == NULL; i++)
		node = (mgf_node_model *)_root_repository[i]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= data;
	return true;
}




}














