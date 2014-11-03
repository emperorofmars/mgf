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

namespace mgf{

struct mgf_data{
	struct mgf_mesh{
		GLuint elementbuffer, vertexbuffer, normalbuffer, *uvbuffer;
		GLuint vao;
	};
	struct mgf_texture{
		std::string name;
		GLuint texturebuffer;
	};
	struct mgf_light{
		std::string name;
		glm::vec3 diffuse;
		float strength;
	};
	struct mgf_material{
		std::string name;
		unsigned int *num_texture;
		glm::vec3 diffuse;
	};
	std::vector<mgf_mesh> _meshes;
	std::vector<mgf_texture> _textures;
	std::vector<mgf_material> _materials;

	bool render_mesh(unsigned int index);
};


class scene{
public:
	scene();
	~scene();

	std::vector<mgf_data> _data;
	std::vector<mgf_node> _root_node;
	std::vector<mgf_node> _root_instances;
private:

};

}

#endif















