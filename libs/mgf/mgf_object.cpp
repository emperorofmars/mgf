/*
**	Author:		Martin Schwarz
**	Name:		mgf_object.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_object.h"

namespace mgf{

//###############################################################  mesh class

//###############################################################  constructor
mesh::mesh(GLuint numVertices, GLuint numFaces, GLuint numUV, GLuint numColors,
		aiVector3D *vertices, aiVector3D **uv, aiFace *faces, aiColor4D **colors,
		unsigned int mat_index){
	vao = 0;
	this->numVertices = numVertices;
	this->numIndices = numFaces * 3;
	this->numUV = numUV;
	this->numColors = numColors;
	this->vertices = vertices;
	this->uv = uv;
	this->colors = colors;
	this->mat_index = mat_index;

	this->indices = new GLuint[this->numIndices];
	this->uvbuffer = new GLuint[this->numUV];

	for(unsigned int i = 0; i < numFaces; i++)
	{
		indices[i*3] = (GLuint)faces[i].mIndices[0];
		indices[i*3+1] = (GLuint)faces[i].mIndices[1];
		indices[i*3+2] = (GLuint)faces[i].mIndices[2];
	}
	//std::cerr << "vertices: " << numVertices  << " faces: " << numFaces << std::endl;

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	for(unsigned int i = 0; i < numUV; i++){
		glGenBuffers(1, &uvbuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, uv[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1 + i);
	}
	//std::cerr << "mesh created" << std::endl;
}

mesh::~mesh(){
	glBindVertexArray(vao);
	glDisableVertexAttribArray(0);
	for(unsigned int i = 0; i < numUV; i++){
		glDisableVertexAttribArray(i + 1);
	}
	glDeleteBuffers(1, &elementbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	for(unsigned int i = 0; i < numUV; i++){
		glDeleteBuffers(1, &uvbuffer[i]);
	}
	glDeleteVertexArrays(1, &vao);
	//std::cerr << "mesh deleted" << std::endl;
}


//###############################################################  material class

//###############################################################  constructor
material::material(aiColor4D diffuse){
	this->diffuse = diffuse;
}

material::~material(){
}


//###############################################################  object class

//###############################################################  constructor
object::object(std::string path, GLuint uniform_trans, GLuint uniform_color){
	this->uniform_trans = uniform_trans;
	this->uniform_color = uniform_color;
	name = path;
	load_file(path);
}

object::~object(){
	for(unsigned int i = 0; i < meshes.size(); i++){
		delete meshes[i];
	}

	std::cerr << "object " << name << " deleted" << std::endl;
}

//###############################################################  load_file
bool object::load_file(std::string path){
	Assimp::Importer importer;
	scene = importer.ReadFile(path.c_str(),
		aiProcess_CalcTangentSpace		|
		aiProcess_Triangulate			|
		aiProcess_JoinIdenticalVertices	|
		aiProcess_SortByPType);

	if(!scene){
		std::cerr << "Importing model " << name << " failed!" << importer.GetErrorString() << std::endl;
		return 0;
	}
	//std::cerr << scene->mNumMeshes << " meshes detected" << std::endl;

	for(unsigned int i = 0; i < scene->mNumMeshes; i++){
		mesh *temp = new mesh(scene->mMeshes[i]->mNumVertices,
			scene->mMeshes[i]->mNumFaces,
			scene->mMeshes[i]->GetNumUVChannels(),
			scene->mMeshes[i]->GetNumColorChannels(),
			scene->mMeshes[i]->mVertices,
			scene->mMeshes[i]->mTextureCoords,
			scene->mMeshes[i]->mFaces,
			scene->mMeshes[i]->mColors,
			scene->mMeshes[i]->mMaterialIndex);

		meshes.push_back(temp);
	}
	//std::cerr << "num Materials: " << scene->mNumMaterials << std::endl;
	for(unsigned int i = 0; i < scene->mNumMaterials; i++){
		aiColor4D diffuse;
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &diffuse);
		//std::cerr << "color diffuse: " << diffuse.r << diffuse.g << diffuse.b << diffuse.a << std::endl;
		material *temp = new material(diffuse);
		materials.push_back(temp);
	}

	std::cerr << "Importing model " << name << " succesful!" << std::endl;

	return true;
}

//###############################################################  transformation matrix
glm::mat4 object::get_trans_mat(){
	return trans;
}

void object::set_trans_mat(glm::mat4 mat){
	trans = mat;
	return;
}

//###############################################################  move
glm::mat4 object::move(glm::vec3 mov){
	trans *= glm::translate(glm::mat4(1.f), mov);
	return trans;
}

glm::mat4 object::rotate(float degrees, glm::vec3 axis){
	trans *= glm::rotate(glm::mat4(1.f), degrees, axis);
	return trans;
}

glm::mat4 object::scale(glm::vec3 scale){
	trans *= glm::scale(glm::mat4(1.f), scale);
	return trans;
}

//###############################################################  render
void object::render(){
	glUniformMatrix4fv(uniform_trans, 1, GL_FALSE, glm::value_ptr(trans));
	for(unsigned int i = 0; i < meshes.size(); i++){
		aiColor4D *tmp = &materials[meshes[i]->mat_index]->diffuse;
		glUniform4fv(uniform_color, 1, glm::value_ptr(glm::vec4(tmp->r, tmp->g, tmp->b, tmp->a)));
        glBindVertexArray(meshes[i]->vao);
		glDrawElements(GL_TRIANGLES, meshes[i]->numIndices * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	}
	return;
}


}












