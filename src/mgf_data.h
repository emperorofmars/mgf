/*
**	Author:		Martin Schwarz
**	Name:		mgf_data.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_DATA
#define MGF_DATA

#include "mgf_include.h"

namespace mgf{

struct mgf_data{
	struct mgf_mesh{
		mgf_mesh();
		~mgf_mesh();

		std::string name;

		GLuint elementbuffer, vertexbuffer, normalbuffer;
		std::vector<GLuint> uvbuffer;
		GLuint vao;

		unsigned int material_index;
		unsigned int num_indices, num_vertices, num_normals;

		bool render_indexed;

		std::vector<unsigned int> indices;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<std::vector<glm::vec3>> texcoords;
	};
	struct mgf_material{
		mgf_material();
		~mgf_material();

		std::string name;

		std::vector<unsigned int> diffuse_texture_index;
		glm::vec4 diffuse;
	};
	struct mgf_texture{
		mgf_texture();
		~mgf_texture();

		std::string name;

		GLuint texturebuffer;
		SDL_Surface *image;
	};
	struct mgf_light{
		mgf_light();
		~mgf_light();

		std::string name;

		glm::vec3 diffuse;
		float strength;
	};

	mgf_data();
	~mgf_data();

	std::vector<mgf_mesh> _meshes;
	std::vector<mgf_material> _materials;
	std::vector<mgf_texture> _textures;
	std::vector<mgf_light> _lights;

	bool render_mesh(unsigned int index);
};

}

#endif // MGF_DATA








