/*
**	Author:		Martin Schwarz
**	Name:		mgf_info.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_info.h"

namespace mgf{

//######################  mgf info
bool mgf_info::_mgf_inited = false;

camera *mgf_info::_current_cam = NULL;
shader_program *mgf_info::_current_prog = NULL;
float mgf_info::_aspect_ratio;

std::vector<window *> mgf_info::_windows;
SDL_GLContext mgf_info::_context;
renderer_enum mgf_info::_renderer = SOFTWARE;


void mgf_info::update_camera(camera *cam){
	_current_cam = cam;
	return;
}

void mgf_info::update_program(shader_program *prog){
	_current_prog = prog;
	//glUseProgram(_current_prog->get_program());
	return;
}

}
