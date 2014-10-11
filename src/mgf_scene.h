/*
**	Author:		Martin Schwarz
**	Name:		mgf_scene.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_SCENE
#define MGF_SCENE

#include "mgf_include.h"
#include "mgf_camera.h"

namespace mgf{

//###############################################################  scene class
class scene{
public:
//###############################################################  constructor
	scene();
	scene(std::string path, bool to_gpu = false);
	~scene();
//###############################################################  do stuff
	bool load(std::string path, bool to_gpu = false);
	bool load_to_gpu();
	bool delete_from_gpu();
	void render(mgf::camera &cam, GLuint program);

	glm::mat4 trans;
private:
	bool load_texture(std::string path, bool to_gpu = false);
	void recursive_render(aiNode *node, mgf::camera &cam, GLuint m_mat, GLuint program);
	void apply_material(unsigned int material_index, GLuint program);

	struct meshes{
		GLuint elementbuffer, vertexbuffer, normalbuffer, *uvbuffer;
		GLuint vao;
	};
	struct materials{
		GLuint *texturebuffer;
	};

//###############################################################  variables
	Assimp::Importer imp;
	const aiScene *_scene;
	std::vector<struct meshes> _meshes;
	std::vector<struct materials> _materials;

	bool loaded_meshes, loaded_textures, loaded_to_gpu;
};

}

#endif
