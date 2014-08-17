/*
**	Author:		Martin Schwarz
**	Name:		mgf_init.h
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


//###############################################################  	mgf class
class mgf{
public:
//###############################################################  constructor
	mgf(unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
			bool input_grabbed = 0, unsigned int monitor = 0, unsigned int framerate = 240);
	~mgf();
//###############################################################  setup
	bool setup_window(unsigned int window_num = 0, unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
			bool input_grabbed = 0, unsigned int monitor = 0, unsigned int framerate = 240);
	unsigned int add_window();
//###############################################################  init
	bool init();
	bool init_window(unsigned int window_num);
//###############################################################  make current
	void current_window(unsigned int window_num);
//###############################################################  swap window
	void swap_window(unsigned int window_num);
	void swap_window();
//###############################################################  close
	void close_window(unsigned int window_num);
//###############################################################  window struct
	struct window{	//window info
		SDL_Window *window;
		unsigned int screen_w, screen_h, monitor, framerate;
		bool fullscreen, input_grabbed;
	};
//###############################################################  variables
	std::vector<struct window> mWindows;
	SDL_GLContext context;
protected:
private:
};

} // mgf
