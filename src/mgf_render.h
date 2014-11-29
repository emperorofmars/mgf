/*
**	Author:		Martin Schwarz
**	Name:		mgf_render.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_RENDER
#define MGF_RENDER

#include "mgf_include.h"
#include "mgf_node.h"
#include "mgf_data.h"
#include "mgf_camera.h"
#include "mgf_shader.h"

namespace mgf{

struct _render_info{
	void update_camera(camera *current_cam);
	void update_program(shader_program *current_prog);

	static camera *_current_cam;
	static shader_program *_current_prog;
	static float aspect_ratio;
};

extern _render_info render_info;

void render(mgf_node_model *node, mgf_data *data, glm::mat4 trans = glm::mat4(1));
void render(mgf_node_model_instance *node, mgf_data *data, glm::mat4  = glm::mat4(1));

bool apply_matrix(glm::mat4 mat, GLuint loc);
void apply_material(unsigned int material_index, mgf_data *data);


}

#endif // MGF_RENDER
