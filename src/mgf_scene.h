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

	bool create_instance_n(std::string origin_name, std::string parent_name, unsigned int index = 0);
	bool create_instance_p(std::string origin_path, std::string parent_path, unsigned int index = 0);

	bool translate_n(std::string name, glm::vec3 data, unsigned int index = 0);
	bool rotate_n(std::string name, float angle, glm::vec3 data, unsigned int index = 0);
	bool scale_n(std::string name, glm::vec3 data, unsigned int index = 0);
	bool multiply_mat_n(std::string name, glm::mat4 data, unsigned int index = 0);

	bool translate_p(std::string path, glm::vec3 data, unsigned int index = 0);
	bool rotate_p(std::string path, float angle, glm::vec3 data, unsigned int index = 0);
	bool scale_p(std::string path, glm::vec3 data, unsigned int index = 0);
	bool multiply_mat_p(std::string path, glm::mat4 data, unsigned int index = 0);

	bool repo_translate(std::string name, glm::vec3 data);
	bool repo_rotate(std::string name, float angle, glm::vec3 data);
	bool repo_scale(std::string name, glm::vec3 data);
	bool repo_multiply_mat(std::string name, glm::mat4 data);

	void render(unsigned int n = 0);
	void render_repository();

	mgf_data *_data;
	mgf_node_model *_root_repository;
	std::vector<mgf_node_model_instance *> _root_instances;
private:
};

}

#endif















