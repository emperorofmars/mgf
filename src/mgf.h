/*
**	Author:		Martin Schwarz
**	Name:		mgf.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/


#ifndef MGF
#define MGF

#ifndef _DEBUG_LEVEL
	#define _DEBUG_LEVEL 2
#endif // _DEBUG_LEVEL

#include "mgf_include.h"

#include "mgf_init.h"
#include "mgf_shader.h"
#include "mgf_camera.h"
#include "mgf_input.h"
#include "mgf_loader.h"
#include "mgf_node.h"
#include "mgf_data.h"
#include "mgf_scene.h"
#include "mgf_render.h"

namespace mgf{

	extern _render_info render_info;

} // mgf

#endif





