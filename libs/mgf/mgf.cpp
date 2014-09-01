/*
**	Author:		Martin Schwarz
**	Name:		mgf.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf.h"

namespace mgf{

void render(std::vector<model *> &models, camera &cam, GLuint program){
	GLuint m_loc = glGetUniformLocation(program, "mvp_mat");

	for(unsigned int i = 0; i < models.size(); i++){
		glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(cam.get_vp() * models[i]->trans));

		for(unsigned int j = 0; j < models[i]->meshes.size(); j++){
			if(models[i]->meshes[j]->has_vertices){
				models[i]->materials[models[i]->meshes[j]->material_index]->use_mtl(program);
				glBindVertexArray(models[i]->meshes[j]->vao);
				glDrawArrays(GL_TRIANGLES, 0, models[i]->meshes[j]->vertices.size() * sizeof(glm::vec3));
			}
		}
	}
	return;
}

} // mgf
