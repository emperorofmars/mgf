/*
**	Author:		Martin Schwarz
**	Name:		mgf_global.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_INCLUDE
#define MGF_INCLUDE

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define _DEBUG_LEVEL 1

enum renderer_enum{
	OPENGL_4_3 = 0,
	OPENGL_3_3 = 1,
	SOFTWARE = 2,
};

enum loader_flags_enum{
	LOAD_NOT_TO_GPU = 0b0010,
	LOAD_TO_DATA = 0b0010,
	LOAD_NO_INDICES = 0b0100,
};

namespace mgf{
	struct mgf_info;
	class window;
	class camera;
	class input;
	class shader_program;
	class mgf_data;
	class node;
	class mgf_node_model;
	class mgf_node_model_instance;
	class scene;
}

#endif








