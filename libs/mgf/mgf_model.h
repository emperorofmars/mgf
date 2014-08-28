/*
**	Author:		Martin Schwarz
**	Name:		mgf_model.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_LIBS
#define MGF_LIBS

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


//###############################################################  obj_material class
class obj_material{
public:
//###############################################################  constructor
	obj_material();
	~obj_material();
//###############################################################  load from file
	bool load_texture(std::string file);
//###############################################################  use_mtl
	void use_mtl(GLuint program);
//###############################################################  variables
	unsigned int material_index;
	glm::vec3 color_diffuse;
	float alpha;
	std::vector<GLuint> texturebuffer;
	std::string name;
protected:
private:
};


//###############################################################  model class
class model{
public:
//###############################################################  constructor
	model(std::string file);
	~model();
//###############################################################  load from file
	bool load_file(std::string file);
	bool load_mtl(std::string file);
	unsigned int get_mtl_index(std::string name);
	bool load_to_buffers();
//###############################################################  transformation matrix
	glm::mat4 get_trans_mat();
	void set_trans_mat(glm::mat4 mat);
//###############################################################  move
	glm::mat4 move(glm::vec3 mov);
	glm::mat4 rotate(float degrees, glm::vec3 axis);
	glm::mat4 scale(glm::vec3 scale);
//###############################################################  render
	void render(GLuint program);
//###############################################################  variables
	std::vector<obj_mesh *> meshes;
	std::vector<obj_material *> materials;
	std::string name;

	GLuint elementbuffer, vertexbuffer, uvbuffer;
	GLuint vao;
	glm::mat4 trans;
protected:
private:
};

}
















