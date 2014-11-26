/*
**	Author:		Martin Schwarz
**	Name:		mgf_scene.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_SCENE
#define MGF_SCENE

#include "mgf_include.h"
#include "mgf_node.h"
#include "mgf_data.h"
#include "mgf_camera.h"
#include "mgf_shader.h"
#include "mgf_render.h"

namespace mgf{

class scene{
public:
	scene();
	~scene();

	mgf_data *_data;
	std::vector<mgf_node_model *> _root_repository;
	std::vector<mgf_node_model_instance *> _root_instances;

	bool translate(std::string name, glm::vec3 data);
	bool rotate(std::string name, float angle, glm::vec3 data);
	bool scale(std::string name, glm::vec3 data);
	bool multiply_mat(std::string name, glm::mat4 data);

	bool repo_translate(std::string name, glm::vec3 data);
	bool repo_rotate(std::string name, float angle, glm::vec3 data);
	bool repo_scale(std::string name, glm::vec3 data);
	bool repo_multiply_mat(std::string name, glm::mat4 data);

	void render();
	void render_repository();
private:
};

}

#endif















