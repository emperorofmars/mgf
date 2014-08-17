/*
**	Author:		Martin Schwarz
**	Name:		mgf_extra.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
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

#endif

namespace mgf{

//###############################################################  convert string to float or double
template<typename T> void stoT(std::string str, std::size_t *pos, T *in);

} // mgf
