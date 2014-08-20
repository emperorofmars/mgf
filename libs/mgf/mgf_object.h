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

#endif

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mgf{

//###############################################################  mesh class
class mesh{
public:
//###############################################################  constructor
	mesh(GLuint numVertices, GLuint numFaces, GLuint numUV, aiVector3D* vertices, aiVector3D** uv, aiFace* faces);
	~mesh();
//###############################################################  variables
	aiVector3D *vertices;
	aiVector3D **uv;
	GLuint *indices;

	GLuint numVertices, numIndices, numMaterials, numUV;
	GLuint elementbuffer, vertexbuffer, *uvbuffer;
	GLuint vao;
protected:
private:
};

//###############################################################  object class
class object{
public:
//###############################################################  constructor
	object(std::string path, GLuint uniform_location = 0);
	~object();
//###############################################################  load from file
	bool load_file(std::string path);
//###############################################################  transformation matrix
	glm::mat4 get_trans_mat();
	void set_trans_mat(glm::mat4 mat);
//###############################################################  render
	void render();
//###############################################################  variables
	std::vector<mesh *> meshes;
	const aiScene* scene;

	GLuint uniform_trans;
	glm::mat4 trans;
protected:
private:
};

}















