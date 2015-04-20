/*
**	Author:		Martin Schwarz
**	Name:		Loader.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_LOADER_H
#define MGF_LOADER_H

#include "../Include.h"
#include "MeshNode.h"
#include "../data/Data.h"

namespace mgf{

class Loader{
public:
	Loader(bool loadIndexed = false);
	~Loader();

	std::shared_ptr<Node> load(const std::string &file);
	void clear();

private:
	std::shared_ptr<Node> loadNodetree(aiNode *ainode);
	bool loadData(const aiScene *scene);
	std::shared_ptr<Mesh> loadMesh(aiMesh *mesh, bool loadToData = false);
	std::shared_ptr<Material> loadMaterial(aiMaterial *material);
	std::shared_ptr<Texture> loadTexture(const std::string &path);
	std::shared_ptr<Light> loadLight(const std::string &path);
	bool loadMeshToGPU(std::shared_ptr<Mesh> mesh);
	bool loadMeshToGPU(std::shared_ptr<Mesh> mgfmesh, aiMesh *aimesh);
	bool loadTextureToGPU(Texture texture);

	std::unordered_map<unsigned int, std::shared_ptr<Mesh>> mLoadedMeshes;
	std::unordered_map<unsigned int, std::shared_ptr<Material>> mLoadedMaterials;

	std::shared_ptr<Data> mData;
	bool mLoadIndexed;

	std::mutex mMutex;
};

/*
mgf::scene *load(std::string path, int flags = 0);
bool load_into_scene(mgf::scene *in_scene, std::string path, int flags = 0);

bool load_to_data(mgf_data *data, const aiScene *ai_scene, std::string path, int flags = 0);
bool load_to_gpu(mgf_data *data, unsigned int oldsize_meshes, int flags = 0);
bool load_to_gpu_from_aiscene(mgf_data *data, const aiScene *ai_scene, unsigned int oldsize_meshes, int flags = 0);

bool load_textures(mgf_data *data, const aiScene *ai_scene, std::string path, unsigned int oldsize_materials, int flags = 0);
int search_texture(mgf_data *data, std::string name);
void construct_nodetree(mgf_node *node, aiNode *ainode, mgf_data *data, unsigned int oldsize_meshes, int flags = 0);

void construct_colboxes(mgf_data *data, unsigned int oldsize_meshes, int flags = 0);
void construct_colboxes(mgf_data *data, const aiScene *ai_scene, unsigned int oldsize_meshes, int flags = 0);
*/
} // mgf

#endif // MGF_LOADER_H












