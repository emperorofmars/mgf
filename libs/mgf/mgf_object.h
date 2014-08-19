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

#endif

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mgf{

//###############################################################  mesh class
class mesh{
public:
//###############################################################  constructor
	mesh(GLuint numVertices, GLuint numFaces, aiVector3D* vertices, aiVector3D** uv, aiFace* faces);
	~mesh();
//###############################################################  render
	void render();
//###############################################################  variables
	aiVector3D *vertices;
	aiVector3D **uv;
	GLuint *indices;

	GLuint numVertices, numIndices, numTextures;
	GLuint elementbuffer, vertexbuffer, uvbuffer;
	GLuint vao;
	char ** textures;
protected:
private:
};

//###############################################################  object class
class object{
public:
//###############################################################  constructor
	object(std::string path);
	~object();
//###############################################################  load from file
	bool load_file(std::string path);
//###############################################################  render
	void render();
//###############################################################  variables
	std::vector<mesh> meshes;
	const aiScene* scene;
	GLuint buffer, elmbuffer;
protected:
private:
};

}















