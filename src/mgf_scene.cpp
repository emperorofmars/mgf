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
				aiProcess_CalcTangentSpace);// |
				//aiProcess_GenNormals);
	if(!_scene){
		#if _DEBUG_LEVEL == 1
			std::cerr << "Importing scene " << _scene->mRootNode->mName.C_Str() << " failed!" << imp.GetErrorString() << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	_name = _scene->mRootNode->mName.C_Str();

	loaded_meshes = true;

	load_texture(path, to_gpu);

	if(to_gpu) load_to_gpu();

	construct_nodetree(NULL, _scene->mRootNode);

	#if _DEBUG_LEVEL == 2
		std::cerr << "Importing scene " << _name << " successful!" << std::endl;
	#endif // _DEBUG_LEVEL
	//std::cerr << "number of meshes: " << _scene->mNumMeshes << std::endl;

	return true;
}

bool scene::load_to_gpu(){
	_meshes.resize(_scene->mNumMeshes);

	if(!loaded_meshes || loaded_to_gpu){
		#if _DEBUG_LEVEL == 1
			std::cerr << "cant load to gpu: not loaded scene or already loaded to gpu" << std::cerr;
		#endif // _DEBUG_LEVEL
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
			for(unsigned int j = 0; j < _scene->mMeshes[i]->GetNumUVChannels(); j++){
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
	#if _DEBUG_LEVEL == 2
		std::cerr << "loaded to buffers successfully: " << _name << std::endl;
	#endif // _DEBUG_LEVEL
	return true;
}

void scene::construct_nodetree(mgf_node *node, aiNode *ainode){
	if(ainode == NULL) return;
	if(node == NULL){
		_root_node = new mgf_node;
		node = _root_node;
		node->_parent_node = NULL;
	}
	//std::cerr << ainode->mName.C_Str() << std::endl;
	if(node->_parent_node != NULL && node->_parent_node->_parent_node != NULL) std::cerr << "parent: " << node->_parent_node->_name << std::endl;
	node->_name = ainode->mName.C_Str();
	node->_num_meshes = ainode->mNumMeshes;
	node->_num_children = ainode->mNumChildren;

	node->_meshes.resize(node->_num_meshes);
	node->_child_nodes.resize(node->_num_children);
	node->_meshes.assign(ainode->mMeshes, ainode->mMeshes + node->_num_meshes);

	node->_trans[0][0] = ainode->mTransformation.a1; node->_trans[0][1] = ainode->mTransformation.b1; node->_trans[0][2] = ainode->mTransformation.c1; node->_trans[0][3] = ainode->mTransformation.d1;
	node->_trans[1][0] = ainode->mTransformation.a2; node->_trans[1][1] = ainode->mTransformation.b2; node->_trans[1][2] = ainode->mTransformation.c2; node->_trans[1][3] = ainode->mTransformation.d2;
	node->_trans[2][0] = ainode->mTransformation.a3; node->_trans[2][1] = ainode->mTransformation.b3; node->_trans[2][2] = ainode->mTransformation.c3; node->_trans[2][3] = ainode->mTransformation.d3;
	node->_trans[3][0] = ainode->mTransformation.a4; node->_trans[3][1] = ainode->mTransformation.b4; node->_trans[3][2] = ainode->mTransformation.c4; node->_trans[3][3] = ainode->mTransformation.d4;

	/*for(unsigned int i = 0; i < 4; i++){
		for(unsigned int j = 0; j < 4; j++) std::cerr << node->_trans[i][j] << " ";
		std::cerr << std::endl;
	}*/

	//node->_trans *= glm::rotate(glm::mat4(1), -(float)M_PI / 2.f, glm::vec3(1.f, 0.f, 0.f));

	for(unsigned int i = 0; i < node->_num_children; i++){
		std::cerr << ainode->mName.C_Str() << ": " << ainode->mChildren[i]->mName.C_Str() << std::endl;
		node->_child_nodes[i] = new mgf_node;
		node->_child_nodes[i]->_parent_node = node;
		construct_nodetree(node->_child_nodes[i], ainode->mChildren[i]);
	}
	return;
}

bool scene::delete_from_gpu(){
	if(!loaded_meshes || !loaded_to_gpu){
		#if _DEBUG_LEVEL == 1
			std::cerr << "cant delete from gpu: not loaded scene or not loaded to gpu" << std::cerr;
		#endif // _DEBUG_LEVEL
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

	for(unsigned int i = path.size() - 1; i > 0; i--){	//get path
		if(path[i] == '/') break;
		else path.erase(path.begin() + i);
	}
	for(unsigned int i = 0; i < _scene->mNumMaterials; i++){
		unsigned int num_tex = _scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		_materials[i].num_texture = new unsigned int[num_tex];	//allocate space for textur references

		for(unsigned int j = 0; j < num_tex; j++){
			std::string newpath(path);
			aiString tmp;
			_scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, j, &tmp, NULL, NULL, NULL, NULL, NULL);	//get texture path relative to loadded file
			newpath.append(tmp.data);	//construct path to texture file

			int tex_ref;
			if((tex_ref = search_texture(newpath)) >= 0){	//if texture already loaded
				_materials[i].num_texture[j] = (unsigned int)tex_ref;
				continue;
			}

			_textures.resize(_textures.size() + 1);	//create new texture struct
			_materials[i].num_texture[j] = _textures.size() - 1;	//reference texture struct in material struct

			glActiveTexture(GL_TEXTURE0 + j);	//create opengl texture object
			glGenTextures(1, &_textures[_textures.size() - 1].texturebuffer);
			glBindTexture(GL_TEXTURE_2D, _textures[_textures.size() - 1].texturebuffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			SDL_Surface *image = IMG_Load(newpath.c_str());	//load texture

			if(image != NULL){	//load it to gpu
				glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, image->w, image->h);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->w, image->h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
				glGenerateMipmap(GL_TEXTURE_2D);

				SDL_FreeSurface(image);
			}
			else{
				#if _DEBUG_LEVEL == 1
					std::cerr << "loading texture failed" << std::endl;
				#endif // _DEBUG_LEVEL
			}
		}
	}
	#if _DEBUG_LEVEL == 2
		std::cerr << "loaded textures successfully" << std::endl;
	#endif // _DEBUG_LEVEL
	return true;
}

int scene::search_texture(std::string &name){
	for(unsigned int i = 0; i < _textures.size(); i++){
		if(name == _textures[i].name) return i;
	}
	return -1;
}

void scene::translate(std::string name, glm::vec3 data){
	mgf_node *node = _root_node->find_node(name);
	if(node == NULL){
		//std::cerr << "could not find: " << name << std::endl;
		return;
	}
	node->_trans *= glm::translate(glm::mat4(1), data);
	return;
}

void scene::rotate(std::string name, float angle, glm::vec3 data){
	mgf_node *node = _root_node->find_node(name);
	if(node == NULL){
		//std::cerr << "could not find: " << name << std::endl;
		return;
	}
	node->_trans *= glm::rotate(glm::mat4(1), angle, data);
	return;
}

void scene::scale(std::string name, glm::vec3 data){
	mgf_node *node = _root_node->find_node(name);
	if(node == NULL){
		//std::cerr << "could not find: " << name << std::endl;
		return;
	}
	node->_trans *= glm::scale(glm::mat4(1), data);
	return;
}

void scene::multiply_mat(std::string name, glm::mat4 data){
	return;
}

void scene::apply_material(unsigned int material_index, mgf::shader_program &program){
	GLuint loc;

	aiColor4D color(0.f, 0.f, 0.f, 0.f);
	aiGetMaterialColor(_scene->mMaterials[material_index], AI_MATKEY_COLOR_DIFFUSE, &color);
	loc = glGetUniformLocation(program.get_program(), "material.color");
	glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(color.r, color.g, color.b, color.a)));

	float alpha = 1.f;
	loc = glGetUniformLocation(program.get_program(), "material.alpha");
	glUniform1f(loc, alpha);

	float has_texture;
	if(_scene->mMaterials[material_index]->GetTextureCount(aiTextureType_DIFFUSE) > 0){
		//std::cerr << _materials[material_index].texturebuffer[0] << std::endl;
		has_texture = 1.f;
		glBindTexture(GL_TEXTURE_2D, _textures[_textures.size() - 1].texturebuffer);
	}
	else{
		has_texture = 0.f;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	loc = glGetUniformLocation(program.get_program(), "material.has_texture");
	glUniform1f(loc, has_texture);
	return;
}

bool scene::apply_mat(glm::mat4 mat, GLuint loc){
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
	return true;
}

bool scene::apply_mat(aiMatrix4x4 mat, GLuint loc){
	float fvmat[16];
	fvmat[0] = mat.a1; fvmat[1] = mat.b1; fvmat[2] = mat.c1; fvmat[3] = mat.d1;
	fvmat[4] = mat.a2; fvmat[5] = mat.b2; fvmat[6] = mat.c2; fvmat[7] = mat.d2;
	fvmat[8] = mat.a3; fvmat[9] = mat.b3; fvmat[10] = mat.c3; fvmat[11] = mat.d3;
	fvmat[12] = mat.a4; fvmat[13] = mat.b4; fvmat[14] = mat.c4; fvmat[15] = mat.d4;

	glUniformMatrix4fv(loc, 1, GL_FALSE, fvmat);
	return true;
}

void scene::render(mgf::camera &cam, mgf::shader_program &program){
	recursive_render(_root_node, glm::mat4(1) ,cam, program);
	return;
}

void scene::recursive_render(mgf_node *node, glm::mat4 oldtrans, mgf::camera &cam, mgf::shader_program &program){
	oldtrans *= node->_trans;
	apply_mat(cam.get_vp(), program.get_vp_mat());
	apply_mat(oldtrans, program.get_m_mat());
	for(unsigned int i = 0; i < node->_num_meshes; i++){
		apply_material(_scene->mMeshes[node->_meshes[i]]->mMaterialIndex, program);

		glBindVertexArray(_meshes[node->_meshes[i]].vao);
		glDrawElements(GL_TRIANGLES, _scene->mMeshes[node->_meshes[i]]->mNumFaces * 3 * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	}

	for(unsigned int i = 0; i < node->_num_children; i ++){
		recursive_render(node->_child_nodes[i], oldtrans, cam, program);
	}
	return;
}

}











