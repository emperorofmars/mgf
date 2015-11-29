/*
**	Author:		Martin Schwarz
**	Name:		LoaderTransparent.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_LOADERTRANSPARENT_H
#define MGF_LOADERTRANSPARENT_H

#include "ILoader.h"
#include "../Include.h"
#include "MeshNode.h"
#include "MeshTransparentNode.h"
#include "../data/Data.h"

namespace mgf{

class LoaderTransparent : public ILoader{
public:
	LoaderTransparent();
	virtual ~LoaderTransparent();

	virtual std::shared_ptr<Node> load(const std::string &file);
	virtual void clear();

private:
	std::shared_ptr<Node> loadNodetree(aiNode *ainode);
	bool loadData(const aiScene *scene);

	std::vector<std::shared_ptr<Mesh>> loadMesh(aiMesh *mesh);
	std::shared_ptr<Material> loadMaterial(aiMaterial *material);
	std::shared_ptr<Texture> loadTexture(const std::string &path);
	std::shared_ptr<Light> loadLight(const std::string &path);

	bool loadMeshToGPU(std::shared_ptr<Mesh> mesh);
	bool loadTextureToGPU(std::shared_ptr<Texture> texture, unsigned int index);

	std::string mFile;

	std::unordered_map<unsigned int, std::vector<std::shared_ptr<Mesh>>> mLoadedMeshes;
	std::unordered_map<unsigned int, std::shared_ptr<Material>> mLoadedMaterials;
	std::unordered_map<std::string, std::shared_ptr<Texture>> mLoadedTextures;

	std::shared_ptr<Data> mData;
	bool mLoadIndexed = false;
	bool mLoadOnlyToGPU = false;

	std::mutex mMutex;
};

} // mgf

#endif // MGF_LOADERTRANSPARENT_H












