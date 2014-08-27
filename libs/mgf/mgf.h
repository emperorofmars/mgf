/*
**	Author:		Martin Schwarz
**	Name:		mgf.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

/*
**	TODO:
**		stuff
*/


#ifndef MGF_LIBS
#define MGF_LIBS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#define GLM_FORCE_RADIANS
#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/type_ptr.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif


#ifndef MGF
#define MGF

#include "mgf_init.h"
#include "mgf_shader.h"
//#include "mgf_object.h"
#include "mgf_model.h"
#include "mgf_camera.h"
#include "mgf_input.h"
#include "mgf_extra.h"

namespace mgf{

} // mgf

#endif





