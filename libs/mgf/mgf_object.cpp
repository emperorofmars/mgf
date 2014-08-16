/*
**	Author:		Martin Schwarz
**	Name:		mgf.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_object.h"

namespace mgf{

//###############################################################  object class

//###############################################################  constructor
mesh::mesh(GLuint numVertices, GLuint numFaces, aiVector3D* vertices, aiVector3D* uv, aiFace* faces){
	vao = 0;
	this->numVertices = numVertices;
	this->numIndices = numFaces * 3;
	this->vertices = vertices;
	this->uv = uv;
	this->indices = new GLuint[this->numIndices];

	for(unsigned int i = 0; i < numFaces; i++)
	{
		indices[i*3] = (GLuint)faces[i].mIndices[0];
		indices[i*3+1] = (GLuint)faces[i].mIndices[1];
		indices[i*3+2] = (GLuint)faces[i].mIndices[2];
	}
	//std::cerr << "vertices: " << numVertices  << " faces: " << numFaces << std::endl;
}

mesh::~mesh(){
}


//###############################################################  object class

//###############################################################  constructor
object::object(std::string path){
	load_file(path);
}

object::~object(){
	std::cerr << "deleting model" << std::endl;
	for(unsigned int i = 0; i < scene->mNumMeshes; i++){
		glDisableVertexAttribArray(0);
		//glDeleteVertexArrays(1, &meshes[i].vao);
	}
	std::cerr << "model deleted" << std::endl;
}

//###############################################################  load_file
bool object::load_file(std::string path){	//all of this is bullshit
	Assimp::Importer importer;
	scene = importer.ReadFile(path.c_str(),
		aiProcess_CalcTangentSpace		|
		aiProcess_Triangulate			|
		aiProcess_JoinIdenticalVertices	|
		aiProcess_SortByPType);

	if(!scene){
		std::cerr << "Importing model failed!" << importer.GetErrorString() << std::endl;
		return 0;
	}

	for(unsigned int i = 0; i < scene->mNumMeshes; i++){
		mesh temp(scene->mMeshes[i]->mNumVertices,
			scene->mMeshes[i]->mNumFaces,
			scene->mMeshes[i]->mVertices,
			scene->mMeshes[i]->mTextureCoords[0],
			scene->mMeshes[i]->mFaces);

		meshes.push_back(temp);

		glGenVertexArrays(1,&(meshes[meshes.size() - 1].vao));
        glBindVertexArray(meshes[meshes.size() - 1].vao);

		glGenBuffers(1, &elmbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elmbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[meshes.size() - 1].numIndices * sizeof(unsigned int), meshes[meshes.size() - 1].indices, GL_STATIC_DRAW);

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, meshes[meshes.size() - 1].numVertices * sizeof(float) * 3, meshes[meshes.size() - 1].vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, meshes[meshes.size() - 1].numVertices * sizeof(float) * 3, meshes[meshes.size() - 1].uv, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}


	std::cerr << "Importing model succesful!" << std::endl;

	return true;
}

void object::render(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elmbuffer);
	for(unsigned int i = 0; i < meshes.size(); i++){
        glBindVertexArray(meshes[i].vao);
		glDrawElements(GL_TRIANGLES, meshes[i].numIndices * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	}
	return;
}


}












