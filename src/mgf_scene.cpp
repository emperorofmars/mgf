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
	_root_repository = NULL;
}

scene::~scene(){
	delete _data;

	delete _root_repository;
	_root_repository = NULL;

	for(unsigned int i = 0; i < _root_instances.size(); i++)
		delete _root_instances[i];
}

void scene::render(unsigned int n){
	_root_instances[n]->render();
	return;
}

void scene::render_repository(){
	_root_repository->render();
	return;
}

bool scene::create_instance_n(std::string origin_name, std::string parent_name, unsigned int index){
	mgf_node_model *node = (mgf_node_model *)_root_repository->find_node(origin_name);
	mgf_node_model_instance *parent = (mgf_node_model_instance *)_root_instances[index]->find_node(parent_name);

	if(node == NULL || parent == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << origin_name << " or: " << parent_name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	if(_root_instances.size() <= index) _root_instances.resize(index + 1);
	parent->add(node->create_instance());
	return true;
}

bool scene::create_instance_p(std::string origin_path, std::string parent_path, unsigned int index){
	mgf_node_model *node = ((mgf_node_model *)_root_repository->get_by_path(origin_path));
	mgf_node_model_instance *parent = ((mgf_node_model_instance *)_root_instances[index]->get_by_path(parent_path));

	if(node == NULL || parent == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << origin_path << " or: " << parent_path << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	if(_root_instances.size() <= index) _root_instances.resize(index + 1);
	parent->add(node->create_instance());
	return true;
}

//######################  scene transform

bool scene::translate_n(std::string name, glm::vec3 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::translate(glm::mat4(1), data);
	return true;
}

bool scene::rotate_n(std::string name, float angle, glm::vec3 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::rotate(glm::mat4(1), angle, data);
	return true;
}

bool scene::scale_n(std::string name, glm::vec3 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::scale(glm::mat4(1), data);
	return true;
}

bool scene::multiply_mat_n(std::string name, glm::mat4 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->find_node(name);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << name << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= data;
	return true;
}

//#################### by path:

bool scene::translate_p(std::string path, glm::vec3 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->get_by_path(path);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << path << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::translate(glm::mat4(1), data);
	return true;
}

bool scene::rotate_p(std::string path, float angle, glm::vec3 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->get_by_path(path);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << path << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::rotate(glm::mat4(1), angle, data);
	return true;
}

bool scene::scale_p(std::string path, glm::vec3 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->get_by_path(path);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << path << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= glm::scale(glm::mat4(1), data);
	return true;
}

bool scene::multiply_mat_p(std::string path, glm::mat4 data, unsigned int index){
	mgf_node_model_instance *node = (mgf_node_model_instance *)_root_instances[index]->get_by_path(path);

	if(node == NULL){
		#if _DEBUG_LEVEL >= 2
			std::cerr << "could not find: " << path << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	node->_trans *= data;
	return true;
}

//######################  scene transform in repo

bool scene::repo_translate(std::string name, glm::vec3 data){
	mgf_node_model *node = NULL;
	node = (mgf_node_model *)_root_repository->find_node(name);

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
	node = (mgf_node_model *)_root_repository->find_node(name);

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
	node = (mgf_node_model *)_root_repository->find_node(name);

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
	node = (mgf_node_model *)_root_repository->find_node(name);

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














