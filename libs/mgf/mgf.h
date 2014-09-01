/*
**	Author:		Martin Schwarz
**	Name:		mgf.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/


#ifndef MGF
#define MGF

#include "mgf_global.h"

#include "mgf_init.h"
#include "mgf_shader.h"
//#include "mgf_object.h"
#include "mgf_model.h"
#include "mgf_camera.h"
#include "mgf_input.h"
#include "mgf_extra.h"

namespace mgf{

void render(std::vector<model *> &models, camera &cam, GLuint program);

} // mgf

#endif





