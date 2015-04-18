/*
**	Author:		Martin Schwarz
**	Name:		Renderer.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Renderer.h"

namespace mgf{

Renderer::Renderer(std::shared_ptr<Window> window,
		 std::shared_ptr<Camera> camera,
		 std::shared_ptr<ShaderProgram> shaderProgram)
{

}

Renderer::~Renderer(){
}

int Renderer::setWindow(std::shared_ptr<Window> window){
	return 0;
}

int Renderer::setCamera(std::shared_ptr<Camera> camera){
	return 0;
}

int Renderer::setShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram){
	return 0;
}

int Renderer::addLight(Light data, glm::mat4 transform){
	return 0;
}

int Renderer::removeLight(Light data){
	return 0;
}

int Renderer::clearLights(){
	return 0;
}

int Renderer::drawMesh(Mesh data, glm::mat4 transform){
	return 0;
}

bool Renderer::good(){
	return false;
}

int Renderer::applyMatrix(glm::mat4 data){
	return 0;
}

int Renderer::applyMaterial(Material data){
	return 0;
}

/*
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
		//std::cerr << "render: " << node->_name << " " << node->_num_children << std::endl;
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

*/

} // mgf
