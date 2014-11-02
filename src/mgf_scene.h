/*
**	Author:		Martin Schwarz
**	Name:		mgf_scene.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_SCENE
#define MGF_SCENE

#include "mgf_include.h"
#include "mgf_node.h"
#include "mgf_camera.h"
#include "mgf_shader.h"

namespace mgf{


struct mgf_meshes{
	GLuint elementbuffer, vertexbuffer, normalbuffer, *uvbuffer;
	GLuint vao;
};

struct mgf_textures{
	GLuint texturebuffer;
	std::string name;
};

struct mgf_materials{
	unsigned int *num_texture;
};


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
	void construct_nodetree(mgf_node *node = NULL, aiNode *ainode = NULL);
	bool delete_from_gpu();
	void render(mgf::camera &cam, mgf::shader_program &program);
//###############################################################  manipulate objects
	void translate(std::string name, glm::vec3 data);
	void rotate(std::string name, float angle, glm::vec3 data);
	void scale(std::string name, glm::vec3 data);
	void multiply_mat(std::string name, glm::mat4 data);

private:
	int search_texture(std::string &name);
	bool load_texture(std::string path, bool to_gpu = false);

	void apply_material(unsigned int material_index, mgf::shader_program &program);
	bool apply_mat(glm::mat4 mat, GLuint loc);
	bool apply_mat(aiMatrix4x4 mat, GLuint loc);

	void recursive_render(mgf_node *node, glm::mat4 oldtrans, mgf::camera &cam, mgf::shader_program &program);

//###############################################################  variables
	Assimp::Importer imp;
	const aiScene *_scene;

	std::vector<struct mgf_meshes> _meshes;
	std::vector<struct mgf_textures> _textures;
	std::vector<struct mgf_materials> _materials;

public: mgf_node *_root_node;
private:
	bool loaded_meshes, loaded_textures, loaded_to_gpu;

	std::string _name;
};

}

#endif
