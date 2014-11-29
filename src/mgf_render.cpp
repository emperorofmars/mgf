/*
**	Author:		Martin Schwarz
**	Name:		mgf_render.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_render.h"

namespace mgf{

//######################  render

void render(mgf_node_model *node, glm::mat4 trans){
	if(!node->_render) return;
	//std::cerr << node->_name << " " << node->_num_meshes << " " << node->_num_children << std::endl;

	trans *= node->_trans;
	render_single_node(node, trans);

	for(unsigned int i = 0; i < node->_num_children; i ++){
		render((mgf_node_model *)node->_child_nodes[i], trans);
	}
	return;
}

void render(mgf_node_model_instance *node, glm::mat4 trans){
	if(!node->_render) return;
	//std::cerr << node->_name << " " << node->_num_children << std::endl;

	trans *= node->_trans;
	if(node->_model != NULL)
		render_single_node(node->_model, trans);

	for(unsigned int i = 0; i < node->_num_children; i ++){
		render((mgf_node_model_instance *)node->_child_nodes[i], trans);
	}
	return;
}

void render_single_node(mgf_node_model *node, glm::mat4 trans){
	apply_matrix(trans, mgf_info::_current_prog->get(MATRIX_MODEL));	//model matrix
	apply_matrix(mgf_info::_current_cam->get_vp(), mgf_info::_current_prog->get(MATRIX_VP));	//view-perspective matrix

	for(unsigned int i = 0; i < node->_num_meshes; i++){
		apply_material(node->_data->_meshes[node->_meshes[i]].material_index, node->_data);

		glBindVertexArray(node->_data->_meshes[node->_meshes[i]].vao);

		if(node->_data->_meshes[node->_meshes[i]].render_indexed == true){
			glDrawElements(GL_TRIANGLES, node->_data->_meshes[node->_meshes[i]].num_indices * sizeof(GLuint), GL_UNSIGNED_INT, 0);
			//std::cerr << "elements" << std::endl;
		}
		else{
			glDrawArrays(GL_TRIANGLES, 0, node->_data->_meshes[node->_meshes[i]].num_vertices * sizeof(GLuint));
			//std::cerr << "arrays" << std::endl;
		}
		glBindVertexArray(0);
	}
	return;
}

bool apply_matrix(glm::mat4 mat, GLuint loc){
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
	return true;
}

void apply_material(unsigned int material_index, mgf_data *data){
	GLuint loc = mgf_info::_current_prog->get(MATERIAL_COLOR_DIFFUSE);
	glUniform4fv(loc, 1, glm::value_ptr(data->_materials[material_index].diffuse));

	float alpha = 1.f;
	loc = mgf_info::_current_prog->get(MATERIAL_ALPHA);
	glUniform1f(loc, alpha);

	float has_texture;
	if(data->_materials[material_index].diffuse_texture_index.size() > 0){
		//std::cerr << "RENDER 01: " << data->_materials[material_index].diffuse_texture_index[0] << std::endl;
		has_texture = 1.f;
		glBindTexture(GL_TEXTURE_2D, data->_textures[data->_materials[material_index].diffuse_texture_index[0]].texturebuffer);
	}
	else{
		//std::cerr << "RENDER 02: no texture" << std::endl;
		has_texture = 0.f;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	loc = mgf_info::_current_prog->get(MATERIAL_HAS_TEXTURE);
	glUniform1f(loc, has_texture);
	return;
}



}
