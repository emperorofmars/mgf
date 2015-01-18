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
enum loader_flags_enum{
	LOAD_NOT_TO_GPU 	= 0b00001,
	LOAD_TO_DATA 		= 0b00010,
	LOAD_NO_INDICES 	= 0b00100,
	LOAD_SWITCH_UP_ZY 	= 0b01000,
	LOAD_NO_B_BOXES     = 0b010000,
};
*/

namespace mgf{

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

}

#endif












