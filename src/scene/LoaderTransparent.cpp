/*
**	Author:		Martin Schwarz
**	Name:		LoaderTransparent.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "LoaderTransparent.h"

#include "../helper/Helper.h"

namespace mgf{

LoaderTransparent::LoaderTransparent(bool loadIndexed, bool loadOnlyToGPU){
	mData.reset(new Data);
	mLoadIndexed = loadIndexed;
	mLoadOnlyToGPU = loadOnlyToGPU;
}

LoaderTransparent::~LoaderTransparent(){
}

void LoaderTransparent::clear(){
	mLoadedMeshes.clear();
	mLoadedMaterials.clear();
	mLoadedTextures.clear();
	mData.reset(new Data);
	mFile = "";
}

std::shared_ptr<Node> LoaderTransparent::load(const std::string &file){
	LOG_F_INFO(MGF_LOG_FILE, "Loading transparent file: ", file);

	mMutex.lock();

	clear();

	mFile = file;

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

std::shared_ptr<Node> LoaderTransparent::loadNodetree(aiNode *ainode){
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
		for(unsigned int j = 0; j < mLoadedMeshes[ainode->mMeshes[i]].size(); j++){
			std::shared_ptr<Mesh> mesh = mLoadedMeshes[ainode->mMeshes[i]][j];
			if(mesh){
				ret->addMesh(mesh);
			}
			else{
				//return NULL;
				ret->addMesh(createCube());
				LOG_F_ERROR(MGF_LOG_FILE, "no valid mesh");
				ret->mTranslation = glm::vec3(rand() % 10, rand() % 10, rand() % 10);
			}
		}
	}

	for(unsigned int i = 0; i < ainode->mNumChildren; i++){
		std::shared_ptr<Node> child = loadNodetree(ainode->mChildren[i]);
		if(child){
			ret->add(child);
		}
		else{
			LOG_F_ERROR(MGF_LOG_FILE, "Creating Nodetree failed!");
			return NULL;
		}
	}

	return ret;
}

bool LoaderTransparent::loadData(const aiScene *scene){
	if(!scene) return false;

	for(unsigned int i = 0; i < scene->mNumMaterials; i++){
		std::shared_ptr<Material> material(loadMaterial(scene->mMaterials[i]));
		if(material){
			mLoadedMaterials[i] = material;
			mData->mMaterials.push_back(material);
		}
	}

	for(unsigned int i = 0; i < scene->mNumMeshes; i++){
		std::vector<std::shared_ptr<Mesh>> mesh(loadMesh(scene->mMeshes[i], mLoadOnlyToGPU));
		if(mesh.size() > 0){
			mLoadedMeshes[i] = mesh;
			//mData->mMeshes.push_back(mesh);
			mLoadedMeshes[i].insert(mLoadedMeshes[i].end(), mesh.begin(), mesh.end());
			for(unsigned int j = 0; j < mesh.size(); j++){
				if(!loadMeshToGPU(mesh[j])){
					LOG_F_ERROR(MGF_LOG_FILE, "Loading Mesh to GPU failed!");
					return false;
				}
			}
		}
		else{
			LOG_F_ERROR(MGF_LOG_FILE, "Loading Mesh failed!");
			return false;
		}
	}

	return true;
}

std::vector<std::shared_ptr<Mesh>> LoaderTransparent::loadMesh(aiMesh *mesh, bool loadToData){
	std::vector<std::shared_ptr<Mesh>> vret;
	if(!mesh) return vret;
	std::shared_ptr<Mesh> ret(new Mesh);

	ret->mMaterial = mLoadedMaterials[mesh->mMaterialIndex];
	ret->mName = mesh->mName.C_Str();

	ret->mNumIndices = mesh->mNumFaces * 3;
	ret->mNumVertices = mesh->mNumVertices;
	ret->mNumNormals = mesh->mNumVertices;

	ret->mRenderIndexed = mLoadIndexed;

	if(!loadToData){
		ret->mIndices.resize(mesh->mNumFaces * 3);
		ret->mVertices.resize(mesh->mNumVertices);
		ret->mNormals.resize(mesh->mNumVertices);
		ret->mUV.resize(mesh->GetNumUVChannels());
		ret->mNumUV.resize(mesh->GetNumUVChannels());

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
			ret->mNumUV[i] = mesh->mNumUVComponents[i];
			for(unsigned int j = 0; j < mesh->mNumUVComponents[i]; j++){
				ret->mUV[i][j][0] = mesh->mTextureCoords[i][j][0];
				ret->mUV[i][j][1] = mesh->mTextureCoords[i][j][1];
				ret->mUV[i][j][2] = mesh->mTextureCoords[i][j][2];
			}
		}
	}

	return vret;
}

std::shared_ptr<Material> LoaderTransparent::loadMaterial(aiMaterial *material){
	std::shared_ptr<Material> ret(new Material);

	aiColor4D col;
	if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &col)){
		ret->mDiffuseColor = glm::vec4(col[0], col[1], col[2], col[3]);
	}

	ret->mDiffuseTextures.resize(material->GetTextureCount(aiTextureType_DIFFUSE));

	std::string path = mFile;
	for(unsigned int i = path.size() - 1; i > 0; i--){
		if(path[i] == '/') break;
		else path.erase(path.begin() + i);
	}

	for(unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++){
		std::string newpath(path);
		aiString tmp;
		material->GetTexture(aiTextureType_DIFFUSE, i, &tmp, NULL, NULL, NULL, NULL, NULL);	//get texture path relative to loadded file
		newpath.append(tmp.data);	//construct path to texture file
		ret->mDiffuseTextures[i] = loadTexture(newpath);

		if(ret->mDiffuseTextures[i]){
			loadTextureToGPU(ret->mDiffuseTextures[i], i + 1);
			if(mLoadOnlyToGPU){
				SDL_FreeSurface(ret->mDiffuseTextures[i]->mImage);
				ret->mDiffuseTextures[i]->mImage = NULL;
			}
		}
	}

	return ret;
}

std::shared_ptr<Texture> LoaderTransparent::loadTexture(const std::string &path){
	std::shared_ptr<Texture> ret(new Texture);

	if(mLoadedTextures[path] != NULL){
		LOG_F_INFO(MGF_LOG_FILE, "Texture already exists: ", path);
		return mLoadedTextures[path];
	}

	ret->mName = path;

	SDL_Surface *image = IMG_Load(path.c_str());
	if(!image){
		LOG_F_ERROR(MGF_LOG_FILE, "Failed to load Testure: ", path);
		return NULL;
	}
	ret->mImage = image;

	mLoadedTextures[path] = ret;

	return ret;
}

std::shared_ptr<Light> LoaderTransparent::loadLight(const std::string &path){
	std::shared_ptr<Light> ret(new Light);
	return ret;
}

bool LoaderTransparent::loadMeshToGPU(std::shared_ptr<Mesh> mesh){
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

	if(mesh->mVertices.size() > 0){
		glGenBuffers(1, &mesh->mVertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), &mesh->mVertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}

	if(mesh->mNormals.size() > 0){
		glGenBuffers(1, &mesh->mNormalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mNormalbuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), &mesh->mNormals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}

	for(unsigned int i = 0; i < mesh->mUVBuffer.size(); i++){
		if(mesh->mUV[i].size() > 0){
			glGenBuffers(1, &mesh->mUVBuffer[i]);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->mUVBuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), &mesh->mUV[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(2 + i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(2 + i);
		}
	}
	glBindVertexArray(0);

	return true;
}

bool LoaderTransparent::loadTextureToGPU(std::shared_ptr<Texture> texture, unsigned int index){
	glActiveTexture(GL_TEXTURE0 + index);	//create opengl texture object
	glGenTextures(1, &texture->mTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, texture->mTextureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if(texture->mImage == NULL){
		texture->mImage = IMG_Load(texture->mName.c_str());
	}

	if(texture->mImage != NULL){	//load it to gpu
		glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, texture->mImage->w, texture->mImage->h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->mImage->w, texture->mImage->h, GL_RGBA, GL_UNSIGNED_BYTE, texture->mImage->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return true;
}


} // mgf



















