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

namespace mgf{

class scene{
public:
	scene();
	~scene();

	mgf_data *_data;
	std::vector<mgf_node *> _root_node;
	std::vector<mgf_node *> _root_instances;


	bool translate(std::string name, glm::vec3 data);
	bool rotate(std::string name, float angle, glm::vec3 data);
	bool scale(std::string name, glm::vec3 data);
	bool multiply_mat(std::string name, glm::mat4 data);

	void render(mgf::camera &cam, mgf::shader_program &program);
private:
	void recursive_render(mgf_node *node, glm::mat4 oldtrans, mgf::camera &cam, mgf::shader_program &program);
	void apply_material(unsigned int material_index, mgf::shader_program &program);
	bool apply_mat(glm::mat4 mat, GLuint loc);
};

}

#endif















