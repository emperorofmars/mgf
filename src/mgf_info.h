/*
**	Author:		Martin Schwarz
**	Name:		mgf_info.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_INFO
#define MGF_INFO

#include "mgf_include.h"

namespace mgf{

struct mgf_info{
	void update_camera(camera *current_cam);
	void update_program(shader_program *current_prog);

	static bool _mgf_inited;

	static camera *_current_cam;
	static shader_program *_current_prog;
	static float _aspect_ratio;

	static std::vector<window *> _windows;
	static SDL_GLContext _context;
	static renderer_enum _renderer;
};

}

#endif // MGF_INFO
