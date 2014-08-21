/*
**	Author:		Martin Schwarz
**	Name:		mgf_object.h
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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif

namespace mgf{

//###############################################################  mesh class
class mesh{
public:
//###############################################################  constructor
	mesh(GLuint numVertices, GLuint numFaces, GLuint numUV, GLuint numColors,
		aiVector3D *vertices, aiVector3D **uv, aiFace *faces, aiColor4D **colors,
		unsigned int mat_index);
	~mesh();
//###############################################################  variables
	aiVector3D *vertices;
	aiVector3D **uv;
	GLuint *indices;
	aiColor4D **colors;

	GLuint numVertices, numIndices, numMaterials, numUV, numColors;
	GLuint elementbuffer, vertexbuffer, *uvbuffer;
	GLuint vao;

	unsigned int mat_index;
protected:
private:
};

//###############################################################  material class
class material{
public:
//###############################################################  constructor
	material(aiColor4D diffuse);
	~material();
//###############################################################  variables
	aiColor4D diffuse;
protected:
private:
};

//###############################################################  object class
class object{
public:
//###############################################################  constructor
	object(std::string path, GLuint uniform_trans = 1, GLuint uniform_color = 2);
	~object();
//###############################################################  load from file
	bool load_file(std::string path);
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
	std::vector<mesh *> meshes;
	std::vector<material *> materials;
	const aiScene* scene;
	std::string name;

	GLuint uniform_trans, uniform_color;
	glm::mat4 trans;
protected:
private:
};

}















