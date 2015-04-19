/*
**	Author:		Martin Schwarz
**	Name:		Loader.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Loader.h"

#include "../helper/Helper.h"

namespace mgf{

Loader::Loader(bool loadIndexed){
	mData.reset(new Data);
	mLoadIndexed = loadIndexed;
}

Loader::~Loader(){
}

void Loader::clear(){
	mLoadedMeshes.clear();
	mData.reset(new Data);
}

std::shared_ptr<Node> Loader::load(const std::string &file){
	LOG_F_INFO(MGF_LOG_FILE, "Loading file: ", file);

	mMutex.lock();

	clear();

	Assimp::Importer imp;
	int impflags = (aiProcess_Triangulate |
				   aiProcess_SortByPType |
				   aiProcess_CalcTangentSpace |
				   aiProcess_GenNormals);
	if(mLoadIndexed) impflags |= aiProcess_JoinIdenticalVertices;

	const aiScene *scene = imp.ReadFile(file.c_str(), impflags);
	if(!scene){
		LOG_F_ERROR(MGF_LOG_FILE, "Loading file: ", file, " failed! ", imp.GetErrorString());
		mMutex.unlock();
		return NULL;
	}

	if(!loadData(scene)){
		LOG_F_ERROR(MGF_LOG_FILE, "Loading data from file: ", file, " failed! ", imp.GetErrorString());
		mMutex.unlock();
		return NULL;
	}

	std::shared_ptr<Node> ret = loadNodetree(scene->mRootNode);
	if(!ret){
		LOG_F_ERROR(MGF_LOG_FILE, "Creating Tree from file: ", file, " failed! ", imp.GetErrorString());
		mMutex.unlock();
		return NULL;
	}

	mMutex.unlock();

	return ret;
}

std::shared_ptr<Node> Loader::loadNodetree(aiNode *ainode){
	if(!ainode) return NULL;

	std::shared_ptr<MeshNode> ret(new MeshNode(ainode->mName.C_Str()));

	ret->mNumChildren = ainode->mNumChildren;

	ret->mTRS[0][0] = ainode->mTransformation.a1; ret->mTRS[0][1] = ainode->mTransformation.b1;
	ret->mTRS[0][2] = ainode->mTransformation.c1; ret->mTRS[0][3] = ainode->mTransformation.d1;
	ret->mTRS[1][0] = ainode->mTransformation.a2; ret->mTRS[1][1] = ainode->mTransformation.b2;
	ret->mTRS[1][2] = ainode->mTransformation.c2; ret->mTRS[1][3] = ainode->mTransformation.d2;
	ret->mTRS[2][0] = ainode->mTransformation.a3; ret->mTRS[2][1] = ainode->mTransformation.b3;
	ret->mTRS[2][2] = ainode->mTransformation.c3; ret->mTRS[2][3] = ainode->mTransformation.d3;
	ret->mTRS[3][0] = ainode->mTransformation.a4; ret->mTRS[3][1] = ainode->mTransformation.b4;
	ret->mTRS[3][2] = ainode->mTransformation.c4; ret->mTRS[3][3] = ainode->mTransformation.d4;

	ret->mTranslation[0] = ret->mTRS[3][0];
	ret->mTranslation[1] = ret->mTRS[3][1];
	ret->mTranslation[2] = ret->mTRS[3][2];

	ret->mTRS[3][0] = 0;
	ret->mTRS[3][1] = 0;
	ret->mTRS[3][2] = 0;

	for(unsigned int i = 0; i < ainode->mNumMeshes; i++){
		std::shared_ptr<Mesh> mesh = mLoadedMeshes[ainode->mMeshes[i]];
		if(mesh){
			ret->addMesh(mesh);
		}
		else{
			//return NULL;
			ret->addMesh(createCube());
			LOG_F_INFO(MGF_LOG_FILE, "no valid mesh");
			ret->mTranslation = glm::vec3(rand() % 10, rand() % 10, rand() % 10);
		}
	}

	for(unsigned int i = 0; i < ainode->mNumChildren; i++){
		std::shared_ptr<Node> child = loadNodetree(ainode->mChildren[i]);
		if(child){
			ret->add(child);
		}
		else{
			return NULL;
		}
	}

	return ret;
}

bool Loader::loadData(const aiScene *scene){
	if(!scene) return false;

	for(unsigned int i = 0; i < scene->mNumMaterials; i++){
		std::shared_ptr<Material> material(loadMaterial(scene->mMaterials[i]));
		if(material){
			mLoadedMaterials[i] = material;
			mData->mMaterials.push_back(material);
		}
	}

	for(unsigned int i = 0; i < scene->mNumMeshes; i++){
		std::shared_ptr<Mesh> mesh(loadMesh(scene->mMeshes[i]));
		if(mesh){
			mLoadedMeshes[i] = mesh;
			mData->mMeshes.push_back(mesh);
			if(!loadMeshToGPU(mesh, scene->mMeshes[i])){
				LOG_F_ERROR(MGF_LOG_FILE, "Loading Mesh to GPU failed!");
				return false;
			}
		}
		else{
			LOG_F_ERROR(MGF_LOG_FILE, "Loading Mesh failed!");
			return false;
		}
	}

	return true;
}

std::shared_ptr<Mesh> Loader::loadMesh(aiMesh *mesh){
	if(!mesh) return NULL;
	std::shared_ptr<Mesh> ret(new Mesh);

	ret->mMaterial = mLoadedMaterials[mesh->mMaterialIndex];
	ret->mName = mesh->mName.C_Str();

	ret->mNumIndices = mesh->mNumFaces * 3;
	ret->mNumVertices = mesh->mNumVertices;
	ret->mNumNormals = mesh->mNumVertices;

	ret->mRenderIndexed = mLoadIndexed;

	ret->mIndices.resize(mesh->mNumFaces * 3);
	ret->mVertices.resize(mesh->mNumVertices);
	ret->mNormals.resize(mesh->mNumVertices);
	ret->mUV.resize(mesh->GetNumUVChannels());

	for(unsigned int i = 0; i < mesh->mNumFaces; i++){
		ret->mIndices[i * 3] = mesh->mFaces[i].mIndices[0];
		ret->mIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		ret->mIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}

	for(unsigned int i = 0; i < mesh->mNumVertices; i++){
		ret->mVertices[i][0] = mesh->mVertices[i][0];
		ret->mVertices[i][1] = mesh->mVertices[i][1];
		ret->mVertices[i][2] = mesh->mVertices[i][2];
	}

	for(unsigned int i = 0; i < mesh->mNumVertices; i++){
		ret->mNormals[i][0] = mesh->mNormals[i][0];
		ret->mNormals[i][1] = mesh->mNormals[i][1];
		ret->mNormals[i][2] = mesh->mNormals[i][2];
	}

	for(unsigned int i = 0; i < mesh->GetNumUVChannels(); i++){
		ret->mUV[i].resize(mesh->mNumUVComponents[i]);
		for(unsigned int j = 0; j < mesh->mNumUVComponents[i]; j++){
			ret->mUV[i][j][0] = mesh->mTextureCoords[i][j][0];
			ret->mUV[i][j][1] = mesh->mTextureCoords[i][j][1];
			ret->mUV[i][j][2] = mesh->mTextureCoords[i][j][2];
		}
	}

	return ret;
}

std::shared_ptr<Material> Loader::loadMaterial(aiMaterial *material){
	std::shared_ptr<Material> ret(new Material);
	aiColor4D col;
	if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &col)){
		ret->mDiffuseColor = glm::vec4(col[0], col[1], col[2], col[3]);
	}
	return ret;
}

std::shared_ptr<Texture> Loader::loadTexture(const std::string &path){
	std::shared_ptr<Texture> ret(new Texture);
	return ret;
}

std::shared_ptr<Light> Loader::loadLight(const std::string &path){
	std::shared_ptr<Light> ret(new Light);
	return ret;
}

bool Loader::loadMeshToGPU(std::shared_ptr<Mesh> mesh){
	if(!mesh) return false;

	glGenVertexArrays(1, &mesh->mVAO);
	glBindVertexArray(mesh->mVAO);

	if(mesh->mNumIndices > 0){
		GLuint *indices = new GLuint[mesh->mNumIndices];

		for(unsigned int i = 0; i < mesh->mNumIndices; i++){
			indices[i] = (GLuint)mesh->mIndices[i];
		}
		glGenBuffers(1, &mesh->mIndexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mNumIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

		delete [] indices;
	}
	else mesh->mIndexbuffer = 0;
/*
	if(data->_meshes[i].positions.size() > 0){
		glGenBuffers(1, &data->_meshes[i].vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i].vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, data->_meshes[i].positions.size() * sizeof(glm::vec3), &data->_meshes[i].positions[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}
	else data->_meshes[i].vertexbuffer = 0;

	if(data->_meshes[i].normals.size() > 0){
		glGenBuffers(1, &data->_meshes[i].normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i].normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, data->_meshes[i].normals.size() * sizeof(glm::vec3), &data->_meshes[i].normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}
	else data->_meshes[i].normalbuffer = 0;

	data->_meshes[i].uvbuffer.resize(data->_meshes[i].texcoords.size() > 0);
	for(unsigned int j = 0; j < data->_meshes[i].uvbuffer.size(); j++){
		if(data->_meshes[i].texcoords[j].size()){
			glGenBuffers(1, &data->_meshes[i].uvbuffer[j]);
			glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i].uvbuffer[j]);
			glBufferData(GL_ARRAY_BUFFER, data->_meshes[i].texcoords[j].size() * sizeof(glm::vec3), &data->_meshes[i].texcoords[j][0], GL_STATIC_DRAW);
			glVertexAttribPointer(2 + i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(2 + i);
		}
		else data->_meshes[i].uvbuffer[j] = 0;
	}
	glBindVertexArray(0);
*/
	return true;
}

bool Loader::loadMeshToGPU(std::shared_ptr<Mesh> mgfmesh, aiMesh *aimesh){
	if(!mgfmesh || !aimesh) return false;

	glGenVertexArrays(1, &mgfmesh->mVAO);
	glBindVertexArray(mgfmesh->mVAO);

	if(aimesh->HasPositions()){
		GLuint *indices = new GLuint[aimesh->mNumFaces * 3];

		for(unsigned int i = 0; i < aimesh->mNumFaces; i++){
			indices[i * 3] = (GLuint)aimesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = (GLuint)aimesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = (GLuint)aimesh->mFaces[i].mIndices[2];
		}
		glGenBuffers(1, &mgfmesh->mIndexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mgfmesh->mIndexbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, aimesh->mNumFaces * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

		delete [] indices;
	}
	else mgfmesh->mIndexbuffer = 0;

	if(aimesh->HasPositions()){
		glGenBuffers(1, &mgfmesh->mVertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mgfmesh->mVertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, aimesh->mNumVertices * sizeof(aiVector3D), aimesh->mVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}
	else mgfmesh->mVertexbuffer = 0;
/*
	if(ai_scene->mMeshes[i]->HasNormals()){
		glGenBuffers(1, &data->_meshes[i + oldsize_meshes].normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i + oldsize_meshes].normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, ai_scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), ai_scene->mMeshes[i]->mNormals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}
	else data->_meshes[i + oldsize_meshes].normalbuffer = 0;

	data->_meshes[i + oldsize_meshes].uvbuffer.resize(ai_scene->mMeshes[i]->GetNumUVChannels());
	for(unsigned int j = 0; j < data->_meshes[i + oldsize_meshes].uvbuffer.size(); j++){
		if(ai_scene->mMeshes[i]->HasTextureCoords(j)){
			glGenBuffers(1, &data->_meshes[i + oldsize_meshes].uvbuffer[j]);
			glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i + oldsize_meshes].uvbuffer[j]);
			glBufferData(GL_ARRAY_BUFFER, ai_scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), ai_scene->mMeshes[i]->mTextureCoords[j], GL_STATIC_DRAW);
			glVertexAttribPointer(2 + i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(2 + i);
		}
		else data->_meshes[i + oldsize_meshes].uvbuffer[j] = 0;
	}
*/
	glBindVertexArray(0);

	return true;
}

bool Loader::loadTextureToGPU(Texture texture){
	return true;
}




/*
mgf::scene *load(std::string path, int flags){
	mgf::scene *new_scene = new mgf::scene;
	load_into_scene(new_scene, path, flags);
	return new_scene;
}

bool load_into_scene(mgf::scene *in_scene, std::string path, int flags){
	if(in_scene == NULL){
		#if _DEBUG_LEVEL >= 1
			std::cerr << "Importing scene failed! in_scene is null!" << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	Assimp::Importer imp;
	int impflags = aiProcess_Triangulate |
				   aiProcess_SortByPType |
				   aiProcess_CalcTangentSpace;// | aiProcess_GenNormals);
	if((flags & LOAD_NO_INDICES) == 0) impflags |= aiProcess_JoinIdenticalVertices;

	const aiScene *ai_scene = imp.ReadFile(path.c_str(), impflags);
	if(!ai_scene){
		#if _DEBUG_LEVEL >= 1
			std::cerr << "Importing scene " << ai_scene->mRootNode->mName.C_Str() << " failed!" << imp.GetErrorString() << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}

	unsigned int oldsize_meshes = in_scene->_data->_meshes.size();
	unsigned int oldsize_materials = in_scene->_data->_materials.size();
	//unsigned int oldsize_textures = in_scene->_data->_textures.size();
	//unsigned int oldsize_lights = in_scene->_data->_lights.size();
	if(oldsize_materials == 0) oldsize_materials = 1;


	if(!load_to_data(in_scene->_data, ai_scene, path, flags)){	//load data struct
		#if _DEBUG_LEVEL >= 1
			std::cerr << "load_to_data failed!" << std::endl;
		#endif // _DEBUG_LEVEL
	}

	if(!load_textures(in_scene->_data, ai_scene, path, oldsize_materials, flags)){
		#if _DEBUG_LEVEL >= 1
			std::cerr << "load_textures failed!" << std::endl;
		#endif // _DEBUG_LEVEL
	}

	if(in_scene->_root_repository == NULL){
		in_scene->_root_repository = new mgf::mgf_node_model();	//create root node
		in_scene->_root_repository->_name = "root";
	}
	//in_scene->_root_repository->construct_from_ainode(ai_scene->mRootNode, in_scene->_data, oldsize_meshes);	//construct nodetree
	mgf::construct_nodetree(in_scene->_root_repository, ai_scene->mRootNode, in_scene->_data, oldsize_meshes, flags);

	if(in_scene->_root_instances.size() > 0){
		delete in_scene->_root_instances[0];
		in_scene->_root_instances[0] = new mgf::mgf_node_model_instance();	//create root node
		in_scene->_root_instances[0]->_name = "root";
		//in_scene->_root_instances.clear();
	}

	if(in_scene->_root_instances.size() == 0){
		in_scene->_root_instances.push_back(new mgf::mgf_node_model_instance());	//create root node
			in_scene->_root_instances[in_scene->_root_instances.size() - 1]->_name = "root";
	}
	if((flags & LOAD_NO_INSTANCES) == 0){
	for(unsigned int i = 0; i < in_scene->_root_repository->_num_children; i++)
		in_scene->_root_instances[in_scene->_root_instances.size() - 1]->
			construct_from_mgf_node((mgf_node_model *)in_scene->_root_repository->_child_nodes[i]);	//construct nodetree
	}
	if((flags & LOAD_NOT_TO_GPU) == 0){
		if((flags & LOAD_SWITCH_UP_ZY) == 0){
			if(!load_to_gpu_from_aiscene(in_scene->_data, ai_scene, oldsize_meshes, flags)){
				#if _DEBUG_LEVEL >= 1
					std::cerr << "load_to_gpu_from_aiscene failed!" << std::endl;
				#endif // _DEBUG_LEVEL
			}
		}
		else{
			if(!load_to_gpu(in_scene->_data, oldsize_meshes, flags)){
				#if _DEBUG_LEVEL >= 1
					std::cerr << "load_to_gpu_from_aiscene failed!" << std::endl;
				#endif // _DEBUG_LEVEL
			}
			if((flags & LOAD_TO_DATA) == 0){
				//delete 3d data
			}
		}
	}

	#if _DEBUG_LEVEL >= 2
		std::cerr << "Importing scene " << in_scene->_root_repository->_child_nodes[in_scene->_root_repository->_child_nodes.size() - 1]->_name << " successful!" << std::endl;
	#endif // _DEBUG_LEVEL
	//std::cerr << "number of meshes: " << ai_scene->mNumMeshes << std::endl;
	return false;
}

bool load_to_data(mgf_data *data, const aiScene *ai_scene, std::string path, int flags){	//fill data struct
	for(unsigned int i = path.size() - 1; i > 0; i--){	//get path
		if(path[i] == '/') break;
		else path.erase(path.begin() + i);
	}

	unsigned int oldsize_meshes = data->_meshes.size();
	unsigned int oldsize_materials = data->_materials.size();
	//unsigned int oldsize_textures = data->_textures.size();
	//unsigned int oldsize_lights = data->_lights.size();
	if(oldsize_materials == 0) oldsize_materials = 1;

//std::cerr << "dazwischen 01: " << data->_meshes.size() << std::endl;
	data->_meshes.resize(oldsize_meshes + ai_scene->mNumMeshes);
//std::cerr << "dazwischen 02: " << data->_meshes.size() << std::endl;
	data->_materials.resize(oldsize_materials + ai_scene->mNumMaterials);

	for(unsigned int i = 0; i < ai_scene->mNumMeshes; i++){	//load meshes
		data->_meshes[i + oldsize_meshes].material_index = ai_scene->mMeshes[i]->mMaterialIndex + oldsize_materials;

		data->_meshes[i + oldsize_meshes].name = ai_scene->mMeshes[i]->mName.data;

		data->_meshes[i + oldsize_meshes].num_indices = ai_scene->mMeshes[i]->mNumFaces * 3;
		data->_meshes[i + oldsize_meshes].num_vertices = ai_scene->mMeshes[i]->mNumVertices;
		data->_meshes[i + oldsize_meshes].num_normals = ai_scene->mMeshes[i]->mNumVertices;

		if((flags & LOAD_NO_INDICES) == 0){
			data->_meshes[i + oldsize_meshes].render_indexed = true;
			//std::cerr << "indexed: true" << std::endl;
		}
		else{
			data->_meshes[i + oldsize_meshes].render_indexed = false;
			//std::cerr << "indexed: false" << std::endl;
		}

		if((flags & (LOAD_TO_DATA | LOAD_SWITCH_UP_ZY)) > 0){	//load mesh data
			data->_meshes[i + oldsize_meshes].indices.resize(ai_scene->mMeshes[i]->mNumFaces * 3);
			data->_meshes[i + oldsize_meshes].positions.resize(ai_scene->mMeshes[i]->mNumVertices);
			data->_meshes[i + oldsize_meshes].normals.resize(ai_scene->mMeshes[i]->mNumVertices);
			data->_meshes[i + oldsize_meshes].texcoords.resize(ai_scene->mMeshes[i]->GetNumUVChannels());

			for(unsigned int j = 0; j < ai_scene->mMeshes[i]->mNumFaces; j++){
				data->_meshes[i + oldsize_meshes].indices[j*3] = ai_scene->mMeshes[i]->mFaces[j].mIndices[0];
				data->_meshes[i + oldsize_meshes].indices[j*3+1] = ai_scene->mMeshes[i]->mFaces[j].mIndices[1];
				data->_meshes[i + oldsize_meshes].indices[j*3+2] = ai_scene->mMeshes[i]->mFaces[j].mIndices[2];
			}

			//collada fix happens here
			if(0 > 0){
			//if((flags & LOAD_SWITCH_UP_ZY) > 0){
				for(unsigned int j = 0; j < ai_scene->mMeshes[i]->mNumVertices; j++){
					data->_meshes[i + oldsize_meshes].positions[j][0] = ai_scene->mMeshes[i]->mVertices[j][0];
					data->_meshes[i + oldsize_meshes].positions[j][1] = ai_scene->mMeshes[i]->mVertices[j][2];
					data->_meshes[i + oldsize_meshes].positions[j][2] = -ai_scene->mMeshes[i]->mVertices[j][1];
				}

				for(unsigned int j = 0; j < ai_scene->mMeshes[i]->mNumVertices; j++){
					data->_meshes[i + oldsize_meshes].normals[j][0] = ai_scene->mMeshes[i]->mNormals[j][0];
					data->_meshes[i + oldsize_meshes].normals[j][1] = ai_scene->mMeshes[i]->mNormals[j][2];
					data->_meshes[i + oldsize_meshes].normals[j][2] = -ai_scene->mMeshes[i]->mNormals[j][1];
				}
			}
			else{
				for(unsigned int j = 0; j < ai_scene->mMeshes[i]->mNumVertices; j++){
					data->_meshes[i + oldsize_meshes].positions[j][0] = ai_scene->mMeshes[i]->mVertices[j][0];
					data->_meshes[i + oldsize_meshes].positions[j][1] = ai_scene->mMeshes[i]->mVertices[j][1];
					data->_meshes[i + oldsize_meshes].positions[j][2] = ai_scene->mMeshes[i]->mVertices[j][2];
				}

				for(unsigned int j = 0; j < ai_scene->mMeshes[i]->mNumVertices; j++){
					data->_meshes[i + oldsize_meshes].normals[j][0] = ai_scene->mMeshes[i]->mNormals[j][0];
					data->_meshes[i + oldsize_meshes].normals[j][1] = ai_scene->mMeshes[i]->mNormals[j][1];
					data->_meshes[i + oldsize_meshes].normals[j][2] = ai_scene->mMeshes[i]->mNormals[j][2];
				}
			}



			for(unsigned int j = 0; j < ai_scene->mMeshes[i]->GetNumUVChannels(); j++){
				data->_meshes[i + oldsize_meshes].texcoords[j].resize(ai_scene->mMeshes[i]->mNumUVComponents[j]);
				for(unsigned int h = 0; h < ai_scene->mMeshes[i]->mNumUVComponents[j]; h++){
					data->_meshes[i + oldsize_meshes].texcoords[j][h][0] = ai_scene->mMeshes[i]->mTextureCoords[j][h][0];
					data->_meshes[i + oldsize_meshes].texcoords[j][h][1] = ai_scene->mMeshes[i]->mTextureCoords[j][h][1];
					data->_meshes[i + oldsize_meshes].texcoords[j][h][2] = ai_scene->mMeshes[i]->mTextureCoords[j][h][2];
				}
			}
		}
		if((flags & LOAD_NO_B_BOXES) == 0 && (flags & LOAD_TO_DATA) == 1){
            construct_colboxes(data, oldsize_meshes, flags);
		}
		else if((flags & (LOAD_NO_B_BOXES | LOAD_TO_DATA)) == 0){
            construct_colboxes(data, ai_scene, oldsize_meshes, flags);
		}
	}

	for(unsigned int i = 0; i < ai_scene->mNumMaterials; i++){	//load materials and textures
		aiColor4D col;
		if(AI_SUCCESS == aiGetMaterialColor(ai_scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &col)){
			data->_materials[i + oldsize_materials].diffuse[0] = col[0];
			data->_materials[i + oldsize_materials].diffuse[1] = col[1];
			data->_materials[i + oldsize_materials].diffuse[2] = col[2];
			data->_materials[i + oldsize_materials].diffuse[3] = col[3];
		}

		//data->_materials[i + oldsize_materials].diffuse_texture_index.resize(ai_scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE));
	}

	return true;
}

bool load_textures(mgf_data *data, const aiScene *ai_scene, std::string path, unsigned int oldsize_materials, int flags){
	for(unsigned int i = path.size() - 1; i > 0; i--){	//get path
		if(path[i] == '/') break;
		else path.erase(path.begin() + i);
	}

	for(unsigned int i = 0; i < ai_scene->mNumMaterials; i++){
		data->_materials[i + oldsize_materials].diffuse_texture_index.resize(ai_scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE));	//allocate space for texture references

		for(unsigned int j = 0; j < data->_materials[i + oldsize_materials].diffuse_texture_index.size(); j++){
			std::string newpath(path);
			aiString tmp;
			ai_scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, j, &tmp, NULL, NULL, NULL, NULL, NULL);	//get texture path relative to loadded file
			newpath.append(tmp.data);	//construct path to texture file

			int tex_ref;
			if((tex_ref = search_texture(data, newpath)) >= 0){	//if texture already loaded
				data->_materials[i + oldsize_materials].diffuse_texture_index[j] = (unsigned int)tex_ref;
				continue;
			}

			data->_textures.resize(data->_textures.size() + 1);	//create new texture struct
			data->_materials[i + oldsize_materials].diffuse_texture_index[j] = data->_textures.size() - 1;	//reference texture struct in material struct
			data->_textures[data->_textures.size() - 1].name = tmp.data;

			glActiveTexture(GL_TEXTURE0 + j);	//create opengl texture object
			glGenTextures(1, &data->_textures[data->_textures.size() - 1].texturebuffer);
			glBindTexture(GL_TEXTURE_2D, data->_textures[data->_textures.size() - 1].texturebuffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			data->_textures[data->_textures.size() - 1].image = IMG_Load(newpath.c_str());	//load texture

			if(data->_textures[data->_textures.size() - 1].image != NULL){	//load it to gpu
				SDL_Surface *image = data->_textures[data->_textures.size() - 1].image;

				glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, image->w, image->h);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->w, image->h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
				glGenerateMipmap(GL_TEXTURE_2D);

				if((flags & 2) == 0){
					SDL_FreeSurface(data->_textures[data->_textures.size() - 1].image);
					data->_textures[data->_textures.size() - 1].image = NULL;
				}
			}
			else{
				#if _DEBUG_LEVEL >= 1
					std::cerr << "loading texture failed" << std::endl;
				#endif // _DEBUG_LEVEL
			}
		}
	}
	#if _DEBUG_LEVEL >= 2
		std::cerr << "loaded textures successfully" << std::endl;
	#endif // _DEBUG_LEVEL
	return true;
}

int search_texture(mgf_data *data, std::string name){
	for(unsigned int i = 0; i < data->_textures.size(); i++){
		if(name == data->_textures[i].name) return i;
	}
	return -1;
}

bool load_to_gpu(mgf_data *data, unsigned int oldsize_meshes, int flags){	//from internal data structures
	if(data == NULL){
		#if _DEBUG_LEVEL >= 1
			std::cerr << "data is NULL" << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}

	for(unsigned int i = oldsize_meshes; i < data->_meshes.size(); i++){	//load all meshes
		glGenVertexArrays(1, &data->_meshes[i].vao);
		glBindVertexArray(data->_meshes[i].vao);

		GLuint *indices = new GLuint[data->_meshes[i].indices.size()];
		for(unsigned int j = 0; j < data->_meshes[i].indices.size(); j++){
			indices[j] = (GLuint)data->_meshes[i].indices[j];
		}

		if(data->_meshes[i].indices.size() > 0){
			glGenBuffers(1, &data->_meshes[i].elementbuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->_meshes[i].elementbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->_meshes[i].indices.size() * sizeof(GLuint), indices, GL_STATIC_DRAW);
		}
		else data->_meshes[i].elementbuffer = 0;

		delete [] indices;

		if(data->_meshes[i].positions.size() > 0){
			glGenBuffers(1, &data->_meshes[i].vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i].vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, data->_meshes[i].positions.size() * sizeof(glm::vec3), &data->_meshes[i].positions[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);
		}
		else data->_meshes[i].vertexbuffer = 0;

		if(data->_meshes[i].normals.size() > 0){
			glGenBuffers(1, &data->_meshes[i].normalbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i].normalbuffer);
			glBufferData(GL_ARRAY_BUFFER, data->_meshes[i].normals.size() * sizeof(glm::vec3), &data->_meshes[i].normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);
		}
		else data->_meshes[i].normalbuffer = 0;

		data->_meshes[i].uvbuffer.resize(data->_meshes[i].texcoords.size() > 0);
		for(unsigned int j = 0; j < data->_meshes[i].uvbuffer.size(); j++){
			if(data->_meshes[i].texcoords[j].size()){
				glGenBuffers(1, &data->_meshes[i].uvbuffer[j]);
				glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i].uvbuffer[j]);
				glBufferData(GL_ARRAY_BUFFER, data->_meshes[i].texcoords[j].size() * sizeof(glm::vec3), &data->_meshes[i].texcoords[j][0], GL_STATIC_DRAW);
				glVertexAttribPointer(2 + i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(2 + i);
			}
			else data->_meshes[i].uvbuffer[j] = 0;
		}
		glBindVertexArray(0);
	}

	#if _DEBUG_LEVEL >= 2
		std::cerr << "loaded to buffers successfully: " << std::endl;
	#endif // _DEBUG_LEVEL
	return true;
}

bool load_to_gpu_from_aiscene(mgf_data *data, const aiScene *ai_scene, unsigned int oldsize_meshes, int flags){	//from assimp data structures
	if(data == NULL || ai_scene == NULL){
		#if _DEBUG_LEVEL >= 1
			std::cerr << "data or ai_scene are NULL" << std::endl;
		#endif // _DEBUG_LEVEL
		return false;
	}
	//data->_meshes.resize(ai_scene->mNumMeshes + oldsize_meshes);
	for(unsigned int i = 0; i < ai_scene->mNumMeshes; i++){	//load all meshes
		glGenVertexArrays(1, &data->_meshes[i + oldsize_meshes].vao);
		glBindVertexArray(data->_meshes[i + oldsize_meshes].vao);

	//std::cerr << "VAO: " << data->_meshes[i + oldsize_meshes].vao << " INDEX: " << i + oldsize_meshes << " SIZE: " << data->_meshes[i + oldsize_meshes].num_vertices << std::endl;

		GLuint *indices = new GLuint[ai_scene->mMeshes[i]->mNumFaces * 3];
		for(unsigned int j = 0; j < ai_scene->mMeshes[i]->mNumFaces; j++){
			indices[j*3] = (GLuint)ai_scene->mMeshes[i]->mFaces[j].mIndices[0];
			indices[j*3+1] = (GLuint)ai_scene->mMeshes[i]->mFaces[j].mIndices[1];
			indices[j*3+2] = (GLuint)ai_scene->mMeshes[i]->mFaces[j].mIndices[2];
		}

		if(ai_scene->mMeshes[i]->HasFaces()){
			glGenBuffers(1, &data->_meshes[i + oldsize_meshes].elementbuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->_meshes[i + oldsize_meshes].elementbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, ai_scene->mMeshes[i]->mNumFaces * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
		}
		else data->_meshes[i + oldsize_meshes].elementbuffer = 0;

		delete [] indices;

		if(ai_scene->mMeshes[i]->HasPositions()){
			glGenBuffers(1, &data->_meshes[i + oldsize_meshes].vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i + oldsize_meshes].vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, ai_scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), ai_scene->mMeshes[i]->mVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);
		}
		else data->_meshes[i + oldsize_meshes].vertexbuffer = 0;

		if(ai_scene->mMeshes[i]->HasNormals()){
			glGenBuffers(1, &data->_meshes[i + oldsize_meshes].normalbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i + oldsize_meshes].normalbuffer);
			glBufferData(GL_ARRAY_BUFFER, ai_scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), ai_scene->mMeshes[i]->mNormals, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);
		}
		else data->_meshes[i + oldsize_meshes].normalbuffer = 0;

		data->_meshes[i + oldsize_meshes].uvbuffer.resize(ai_scene->mMeshes[i]->GetNumUVChannels());
		for(unsigned int j = 0; j < data->_meshes[i + oldsize_meshes].uvbuffer.size(); j++){
			if(ai_scene->mMeshes[i]->HasTextureCoords(j)){
				glGenBuffers(1, &data->_meshes[i + oldsize_meshes].uvbuffer[j]);
				glBindBuffer(GL_ARRAY_BUFFER, data->_meshes[i + oldsize_meshes].uvbuffer[j]);
				glBufferData(GL_ARRAY_BUFFER, ai_scene->mMeshes[i]->mNumVertices * sizeof(aiVector3D), ai_scene->mMeshes[i]->mTextureCoords[j], GL_STATIC_DRAW);
				glVertexAttribPointer(2 + i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(2 + i);
			}
			else data->_meshes[i + oldsize_meshes].uvbuffer[j] = 0;
		}
		glBindVertexArray(0);
	}

	#if _DEBUG_LEVEL >= 2
		std::cerr << "loaded to buffers successfully: " << std::endl;
	#endif // _DEBUG_LEVEL
	return true;
}

void construct_nodetree(mgf_node *node, aiNode *ainode, mgf_data *data, unsigned int oldsize_meshes, int flags){
	if(ainode == NULL) return;
	//std::cerr << _name << " CONSTRUCT: " << ainode->mName.C_Str() << std::endl;

	mgf_node_model *newnode = new mgf_node_model;
	newnode->_parent_node = node;
	node->_child_nodes.push_back(newnode);

	//if(_child_nodes[0] == newnode)std::cerr << "SAME AS FIRST" << std::endl;

	node->_num_children = node->_child_nodes.size();

	newnode->_data = data;
	newnode->_name = ainode->mName.C_Str();
	newnode->_num_meshes = ainode->mNumMeshes;
	newnode->_num_children = ainode->mNumChildren;

	newnode->_trans[0][0] = ainode->mTransformation.a1; newnode->_trans[0][1] = ainode->mTransformation.b1;	//get matrix
	newnode->_trans[0][2] = ainode->mTransformation.c1; newnode->_trans[0][3] = ainode->mTransformation.d1;
	newnode->_trans[1][0] = ainode->mTransformation.a2; newnode->_trans[1][1] = ainode->mTransformation.b2;
	newnode->_trans[1][2] = ainode->mTransformation.c2; newnode->_trans[1][3] = ainode->mTransformation.d2;
	newnode->_trans[2][0] = ainode->mTransformation.a3; newnode->_trans[2][1] = ainode->mTransformation.b3;
	newnode->_trans[2][2] = ainode->mTransformation.c3; newnode->_trans[2][3] = ainode->mTransformation.d3;
	newnode->_trans[3][0] = ainode->mTransformation.a4; newnode->_trans[3][1] = ainode->mTransformation.b4;
	newnode->_trans[3][2] = ainode->mTransformation.c4; newnode->_trans[3][3] = ainode->mTransformation.d4;

	newnode->_meshes.resize(ainode->mNumMeshes);
	for(unsigned int i = 0; i < newnode->_meshes.size(); i++)
		newnode->_meshes[i] = ainode->mMeshes[i] + oldsize_meshes;

	if(newnode->_meshes.size() > 0){
		newnode->bbox_vol = data->_meshes[newnode->_meshes[0]].bbox_vol;
		newnode->bbox_hur = data->_meshes[newnode->_meshes[0]].bbox_hur;

		for(unsigned int i = 0; i < newnode->_meshes.size(); i++){
			for(unsigned int j = 0; j < 3; j++){
				if(newnode->bbox_vol[j] < data->_meshes[newnode->_meshes[i]].bbox_vol[j]) newnode->bbox_vol[j] = data->_meshes[newnode->_meshes[i]].bbox_vol[j];
				if(newnode->bbox_hur[j] > data->_meshes[newnode->_meshes[i]].bbox_hur[j]) newnode->bbox_hur[j] = data->_meshes[newnode->_meshes[i]].bbox_hur[j];
			}
		}
	}

	for(unsigned int i = 0; i < ainode->mNumChildren; i++)
		newnode->construct_from_ainode(ainode->mChildren[i], data, oldsize_meshes);

	if(newnode->_num_children > 0){
		newnode->bbox_vol = ((mgf_node_model *)newnode->_child_nodes[0])->bbox_vol;
		newnode->bbox_hur = ((mgf_node_model *)newnode->_child_nodes[0])->bbox_hur;

		for(unsigned int i = 0; i < newnode->_num_children; i++){
			for(unsigned int j = 0; j < 3; j++){
				if(newnode->bbox_vol[j] < ((mgf_node_model *)newnode->_child_nodes[i])->bbox_vol[j]) newnode->bbox_vol[j] = ((mgf_node_model *)newnode->_child_nodes[i])->bbox_vol[j];
				if(newnode->bbox_hur[j] > ((mgf_node_model *)newnode->_child_nodes[i])->bbox_hur[j]) newnode->bbox_hur[j] = ((mgf_node_model *)newnode->_child_nodes[i])->bbox_hur[j];
			}
		}
	}

	//std::cerr << "BBOX: " << newnode->_name << " " << newnode->bbox_vol[0] << " " << newnode->bbox_vol[1] << " " << newnode->bbox_vol[2]
	//				<< " " << newnode->bbox_hur[0] << " " << newnode->bbox_hur[1] << " " << newnode->bbox_hur[2] << std::endl;
	return;
}

void construct_colboxes(mgf_data *data, unsigned int oldsize_meshes, int flags){
    if(data == NULL) return;
    for(unsigned int i = oldsize_meshes; i < data->_meshes.size(); i++){
        glm::vec3 max_vol = data->_meshes[i].positions[0];
        glm::vec3 max_hur = data->_meshes[i].positions[0];
        for(unsigned int j = 1; i < data->_meshes[i].positions.size(); i++){
            if(data->_meshes[i].positions[j][0] > max_vol[0]) max_vol[0] = data->_meshes[i].positions[j][0];
            if(data->_meshes[i].positions[j][1] > max_vol[1]) max_vol[1] = data->_meshes[i].positions[j][1];
            if(data->_meshes[i].positions[j][2] > max_vol[2]) max_vol[2] = data->_meshes[i].positions[j][2];
            if(data->_meshes[i].positions[j][0] < max_hur[0]) max_hur[0] = data->_meshes[i].positions[j][0];
            if(data->_meshes[i].positions[j][1] < max_hur[1]) max_hur[1] = data->_meshes[i].positions[j][1];
            if(data->_meshes[i].positions[j][2] < max_hur[2]) max_hur[2] = data->_meshes[i].positions[j][2];
        }
        data->_meshes[i].bbox_vol = max_vol;
        data->_meshes[i].bbox_hur = max_hur;
    }
    return;
}

void construct_colboxes(mgf_data *data, const aiScene *ai_scene, unsigned int oldsize_meshes, int flags){
    if(data == NULL || ai_scene == NULL) return;
    for(unsigned int i = 0; i < ai_scene->mNumMeshes; i++){
        glm::vec3 max_vol(ai_scene->mMeshes[i]->mVertices[0][0], ai_scene->mMeshes[i]->mVertices[0][1], ai_scene->mMeshes[i]->mVertices[0][2]);
        glm::vec3 max_hur(ai_scene->mMeshes[i]->mVertices[0][0], ai_scene->mMeshes[i]->mVertices[0][1], ai_scene->mMeshes[i]->mVertices[0][2]);
        for(unsigned int j = 1; j < ai_scene->mMeshes[i]->mNumVertices; j++){
            if(ai_scene->mMeshes[i]->mVertices[j][0] > max_vol[0]) max_vol[0] = ai_scene->mMeshes[i]->mVertices[j][0];
            if(ai_scene->mMeshes[i]->mVertices[j][1] > max_vol[1]) max_vol[1] = ai_scene->mMeshes[i]->mVertices[j][1];
            if(ai_scene->mMeshes[i]->mVertices[j][2] > max_vol[2]) max_vol[2] = ai_scene->mMeshes[i]->mVertices[j][2];
            if(ai_scene->mMeshes[i]->mVertices[j][0] < max_hur[0]) max_hur[0] = ai_scene->mMeshes[i]->mVertices[j][0];
            if(ai_scene->mMeshes[i]->mVertices[j][1] < max_hur[1]) max_hur[1] = ai_scene->mMeshes[i]->mVertices[j][1];
            if(ai_scene->mMeshes[i]->mVertices[j][2] < max_hur[2]) max_hur[2] = ai_scene->mMeshes[i]->mVertices[j][2];
        }
        data->_meshes[i + oldsize_meshes].bbox_vol = max_vol;
        data->_meshes[i + oldsize_meshes].bbox_hur = max_hur;
    }
    return;
}
*/
}



















