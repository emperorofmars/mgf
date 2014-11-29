/*
**	Author:		Martin Schwarz
**	Name:		mgf_window.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_WINDOW
#define MGF_WINDOW

#include "mgf_include.h"
#include "mgf_info.h"

/*enum renderer_enum{
	OPENGL_4_3 = 0,
	OPENGL_3_3 = 1,
	SOFTWARE = 2,
};*/

namespace mgf{

//###############################################################  	window class
class window{
public:
	window(unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
			unsigned int monitor = 0, bool input_grabbed = 0, bool vsync = 0);
	~window();

	//bool setup_window(unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
	//				unsigned int monitor = 0, bool input_grabbed = 0, bool vsync = 0);
	bool init();
	void close();

	void use();
	void swap();
	float get_aspect_ratio();

	friend bool mgf_init(renderer_enum renderer);
	friend void mgf_close();

private:
	SDL_Window *_window;
	unsigned int _screen_w, _screen_h, _monitor;
	bool _fullscreen, _input_grabbed, _vsync;
	bool _inited;
};

//###############################################################  init
bool mgf_init(renderer_enum renderer);
void mgf_close();


} // mgf

#endif // MGF_WINDOW
