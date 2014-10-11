/*
**	Author:		Martin Schwarz
**	Name:		mgf_scene.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_scene.h"

namespace mgf{


//###############################################################  scene class

//###############################################################  constructor
scene::scene(){
	loaded_meshes = false;
	loaded_textures = false;
	loaded_to_gpu = false;
}

scene::scene(std::string path, bool to_gpu){
	loaded_meshes = false;
	loaded_textures = false;
	loaded_to_gpu = false;
	load(path, to_gpu);
}

scene::~scene(){
	for(unsigned int i = 0; i < _meshes.size(); i ++){
		glBindVertexArray(_meshes[i].vao);
		glDeleteVertexArrays(1, &_meshes[i].vao);

		if(_meshes[i].elementbuffer > 0) glDeleteBuffers(1, &_meshes[i].elementbuffer);
		if(_meshes[i].vertexbuffer > 0) glDeleteBuffers(1, &_meshes[i].vertexbuffer);
		if(_meshes[i].normalbuffer > 0) glDeleteBuffers(1, &_meshes[i].normalbuffer);
		for(unsigned int j = 0; j < _scene->mMeshes[i]->GetNumUVChannels(); j++)
				glDeleteBuffers(1, &_meshes[i].uvbuffer[j]);

		delete[] _meshes[i].uvbuffer;
	}
}

bool scene::load(std::string path, bool to_gpu){
	_scene = imp.ReadFile(path.c_str(),
				aiProcess_JoinIdenticalVertices |
				aiProcess_Triangulate |
				aiProcess_SortByPType |
				aiProcess_CalcTangentSpace |
				aiProcess_GenNormals);
	if(!_scene){
		std::cerr << "Importing scene " << _scene->mRootNode->mName.C_Str() << " failed!" << imp.GetErrorString() << std::endl;
		return false;
	}
	loaded_meshes = true;

	load_texture(path, to_gpu);

	if(to_gpu) load_to_gpu();

	std::cerr << "Importing scene " << _scene->mRootNode->mName.C_Str() << " succesful!" << std::endl;
	//std::cerr << "number of meshes: " << _scene->mNumMeshes << std::endl;

	return true;
}

bool scene::load_to_gpu(){
	_meshes.resize(_scene->mNumMeshes);

	if(!loaded_meshes || loaded_to_gpu){
		std::cerr << "cant load to gpu: not loaded scene or already loaded to gpu" << std::cerr;
		return false;
	}

	for(unsigned int i = 0; i < _scene->mNumMeshes; i++){
		glGenVertexArrays(1, &_meshes[i].vao);
		glBindVertexArray(_meshes[i].vao);

		GLuint *indices = new GLuint[_scene->mMeshes[i]->mNumFaces * 3];
		for(unsigned int j = 0; j < _scene->mMeshes[i]->mNumFaces; j++){
			indices[j*3] = (GLuint)_scene->mMeshes[i]->mFaces[j].mIndices[0];
			indices[j*3+1] = (GLuint)_scene->mMeshes[i]->mFaces[j].mIndices[1];
			indices[j*3+2] = (GLuint)_scene->mMeshes[i]->mFaces[j].mIndices[2];
		}

		if(_scene->mMeshes[i]->HasFaces()){
			glGenBuffers(1, &_meshes[i].elementbuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshes[i].elementbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _scene->mMeshes[i]->mNumFaces * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
		}
		else _meshes[i].elementbuffer = 0;

		if(_scene->mMeshes[i]->HasPositions()){
			glGenBuffers(1, &_meshes[i].vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, _meshes[i].vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, _scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), _scene->mMeshes[i]->mVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);
		}
		else _meshes[i].vertexbuffer = 0;

		if(_scene->mMeshes[i]->HasNormals()){
			glGenBuffers(1, &_meshes[i].normalbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, _meshes[i].normalbuffer);
			glBufferData(GL_ARRAY_BUFFER, _scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), _scene->mMeshes[i]->mNormals, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);
		}
		else _meshes[i].normalbuffer = 0;

		if(_scene->mMeshes[i]->GetNumUVChannels() > 0){
			_meshes[i].uvbuffer = new GLuint[_scene->mMeshes[i]->GetNumUVChannels()];
			for(int j = 0; j < _scene->mMeshes[i]->GetNumUVChannels(); j++){
				if(_scene->mMeshes[i]->HasTextureCoords(j)){
					glGenBuffers(1, &_meshes[i].uvbuffer[j]);
					glBindBuffer(GL_ARRAY_BUFFER, _meshes[i].uvbuffer[j]);
					glBufferData(GL_ARRAY_BUFFER, _scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), _scene->mMeshes[i]->mTextureCoords[j], GL_STATIC_DRAW);
					glVertexAttribPointer(2 + i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
					glEnableVertexAttribArray(2 + i);
				}
				else _meshes[i].uvbuffer[j] = 0;
			}
		}
		else _meshes[i].uvbuffer = NULL;
	}
	loaded_to_gpu = true;
	//std::cerr << "loaded to buffers successfully: " << _scene->mRootNode->mName.C_Str() << std::endl;
	return true;
}

bool scene::delete_from_gpu(){
	if(!loaded_meshes || !loaded_to_gpu){
		std::cerr << "cant delete from gpu: not loaded scene or not loaded to gpu" << std::cerr;
		return false;
	}
	for(unsigned int i = 0; i < _meshes.size(); i ++){
		glBindVertexArray(_meshes[i].vao);
		glDeleteVertexArrays(1, &_meshes[i].vao);

		if(_meshes[i].elementbuffer > 0) glDeleteBuffers(1, &_meshes[i].elementbuffer);
		if(_meshes[i].vertexbuffer > 0) glDeleteBuffers(1, &_meshes[i].vertexbuffer);
		if(_meshes[i].normalbuffer > 0) glDeleteBuffers(1, &_meshes[i].normalbuffer);
		for(unsigned int j = 0; j < _scene->mMeshes[i]->GetNumUVChannels(); j++)
				glDeleteBuffers(1, &_meshes[i].uvbuffer[j]);

		delete[] _meshes[i].uvbuffer;
	}
	loaded_to_gpu = false;
	return true;
}

bool scene::load_texture(std::string path, bool to_gpu){
	_materials.resize(_scene->mNumMaterials);

	for(unsigned int i = path.size() - 1; i > 0; i--){
		if(path[i] == '/') break;
		else path.erase(path.begin() + i);
	}
	for(unsigned int i = 0; i < _scene->mNumMaterials; i++){
		unsigned int num_tex = _scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		_materials[i].texturebuffer = new GLuint[num_tex];

		for(unsigned int j = 0; j < num_tex; j++){
			std::string newpath(path);
			aiString tmp;
			_scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, j, &tmp, NULL, NULL, NULL, NULL, NULL);
			newpath.append(tmp.data);

			glActiveTexture(GL_TEXTURE0 + j);
			glGenTextures(1, &_materials[i].texturebuffer[j]);
			glBindTexture(GL_TEXTURE_2D, _materials[i].texturebuffer[j]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			SDL_Surface *image = IMG_Load(newpath.c_str());
			if(image != NULL){
				glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, image->w, image->h);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->w, image->h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
				glGenerateMipmap(GL_TEXTURE_2D);

				SDL_FreeSurface(image);
			}
			else{
				std::cerr << "loading texture failed" << std::endl;
			}
		}
	}
	std::cerr << "loaded textures successfully" << std::endl;
	return true;
}

void scene::apply_material(unsigned int material_index, GLuint program){
	GLuint loc;

	aiColor4D color(0.f, 0.f, 0.f, 0.f);
	aiGetMaterialColor(_scene->mMaterials[material_index], AI_MATKEY_COLOR_DIFFUSE, &color);
	loc = glGetUniformLocation(program, "material.color");
	glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(color.r, color.g, color.b, color.a)));

	float alpha = 1.f;
	loc = glGetUniformLocation(program, "material.alpha");
	glUniform1f(loc, alpha);

	float has_texture;
	if(_scene->mMaterials[material_index]->GetTextureCount(aiTextureType_DIFFUSE) > 0){
		//std::cerr << _materials[material_index].texturebuffer[0] << std::endl;
		has_texture = 1.f;
		glBindTexture(GL_TEXTURE_2D, _materials[material_index].texturebuffer[0]);
	}
	else{
		has_texture = 0.f;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	loc = glGetUniformLocation(program, "material.has_texture");
	glUniform1f(loc, has_texture);
	return;
}

void scene::render(mgf::camera &cam, GLuint program){
	GLuint mvp_mat = glGetUniformLocation(program, "mvp_mat");
	recursive_render(_scene->mRootNode, cam, mvp_mat, program);
	return;
}

void scene::recursive_render(aiNode *node, mgf::camera &cam, GLuint mvp_mat, GLuint program){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		apply_material(_scene->mMeshes[node->mMeshes[i]]->mMaterialIndex, program);
		glUniformMatrix4fv(mvp_mat, 1, GL_FALSE, glm::value_ptr(cam.get_vp() * trans));
		glBindVertexArray(_meshes[node->mMeshes[i]].vao);
		glDrawElements(GL_TRIANGLES, _scene->mMeshes[node->mMeshes[i]]->mNumFaces * 3 * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	}
	for(unsigned int i = 0; i < node->mNumChildren; i ++){
		recursive_render(node->mChildren[i], cam, mvp_mat, program);
	}
	return;
}

}











