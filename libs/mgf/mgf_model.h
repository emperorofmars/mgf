/*
**	Author:		Martin Schwarz
**	Name:		mgf_model.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_LIBS
#define MGF_LIBS

#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#define GLM_FORCE_RADIANS
#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/type_ptr.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"

#endif

namespace mgf{


//###############################################################  mesh class
class obj_mesh{
public:
//###############################################################  constructor
	obj_mesh();
	~obj_mesh();
//###############################################################  variables
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> texcoords;
	unsigned int material_index;
	std::string name;
	bool has_vertices, has_normals, has_texcoords;

	GLuint elementbuffer, vertexbuffer, uvbuffer;
	GLuint vao;
protected:
private:
};

//###############################################################  model class
class model{
public:
//###############################################################  constructor
	model(std::string file, GLuint uniform_trans = 1, GLuint uniform_color = 2);
	~model();
//###############################################################  load from file
	bool load_file(std::string file);
	bool load_to_buffers();
//###############################################################  transformation matrix
	glm::mat4 get_trans_mat();
	void set_trans_mat(glm::mat4 mat);
//###############################################################  move
	glm::mat4 move(glm::vec3 mov);
	glm::mat4 rotate(float degrees, glm::vec3 axis);
	glm::mat4 scale(glm::vec3 scale);
//###############################################################  render
	void render();
//###############################################################  variables
	std::vector<obj_mesh *> meshes;
	std::string name;

	GLuint elementbuffer, vertexbuffer, uvbuffer;
	GLuint vao;

	GLuint uniform_trans, uniform_color;
	glm::mat4 trans;
protected:
private:
};

}
















