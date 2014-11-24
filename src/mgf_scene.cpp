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

void scene::render(mgf::camera &cam, mgf::shader_program &program){
	for(unsigned int i = 0; i < _root_node.size(); i++) recursive_render(_root_node[i], glm::mat4(1) ,cam, program);
	return;
}

void scene::recursive_render(mgf_node *node, glm::mat4 oldtrans, mgf::camera &cam, mgf::shader_program &program){
	oldtrans *= node->_trans;
	apply_mat(cam.get_vp(), program.get_vp_mat());	//view-perspective matrix
	apply_mat(oldtrans, program.get_m_mat());	//model matrix
	for(unsigned int i = 0; i < node->_num_meshes; i++){
		apply_material(_data->_meshes[node->_meshes[i]].material_index, program);

		glBindVertexArray(_data->_meshes[node->_meshes[i]].vao);
		glDrawElements(GL_TRIANGLES, _data->_meshes[node->_meshes[i]].num_indices * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	}

	for(unsigned int i = 0; i < node->_num_children; i ++){
		recursive_render(node->_child_nodes[i], oldtrans, cam, program);
	}
	return;
}

bool scene::apply_mat(glm::mat4 mat, GLuint loc){
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
	return true;
}

void scene::apply_material(unsigned int material_index, mgf::shader_program &program){
	GLuint loc = glGetUniformLocation(program.get_program(), "material.color");
	glUniform4fv(loc, 1, glm::value_ptr(_data->_materials[material_index].diffuse));

	float alpha = 1.f;
	loc = glGetUniformLocation(program.get_program(), "material.alpha");
	glUniform1f(loc, alpha);

	float has_texture;
	if(_data->_materials[material_index].diffuse_texture_index.size() > 0){
		//std::cerr << "RENDER 01: " << _data->_materials[material_index].diffuse_texture_index[0] << std::endl;
		has_texture = 1.f;
		glBindTexture(GL_TEXTURE_2D, _data->_textures[_data->_materials[material_index].diffuse_texture_index[0]].texturebuffer);
	}
	else{
		//std::cerr << "RENDER 02: no texture" << std::endl;
		has_texture = 0.f;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	loc = glGetUniformLocation(program.get_program(), "material.has_texture");
	glUniform1f(loc, has_texture);
	return;
}


}
