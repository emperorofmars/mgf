/*
**	Author:		Martin Schwarz
**	Name:		mgf_render.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_RENDER
#define MGF_RENDER

#include "mgf_include.h"
#include "mgf_info.h"
#include "mgf_node.h"
#include "mgf_data.h"
#include "mgf_camera.h"
#include "mgf_shader.h"

namespace mgf{

void render(mgf_node_model *node, mgf_data *data, glm::mat4 trans = glm::mat4(1));
void render(mgf_node_model_instance *node, mgf_data *data, glm::mat4  = glm::mat4(1));

bool apply_matrix(glm::mat4 mat, GLuint loc);
void apply_material(unsigned int material_index, mgf_data *data);


}

#endif // MGF_RENDER
