/*
**	Author:		Martin Schwarz
**	Name:		mgf_loader.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_LOADER
#define MGF_LOADER

#include "mgf_include.h"
#include "mgf_scene.h"
#include "mgf_data.h"
#include "mgf_node.h"

/*

flags:
1 = not_to_gpu
2 = to_data

*/

namespace mgf{

mgf::scene *load(std::string path, int flags = 0);
bool load_into_scene(mgf::scene *in_scene, std::string path, int flags = 0);

bool load_to_data(mgf_data *data, const aiScene *ai_scene, std::string path, int flags = 0);
bool load_to_gpu(mgf_data *data, int flags = 0);
bool load_to_gpu_from_aiscene(mgf_data *data, const aiScene *ai_scene, unsigned int oldsize_meshes, int flags = 0);

bool load_textures(mgf_data *data, const aiScene *ai_scene, std::string path, unsigned int oldsize_materials, int flags = 0);
int search_texture(mgf_data *data, std::string name);
//void construct_nodetree(mgf_node *node, aiNode *ainode);

}

#endif












