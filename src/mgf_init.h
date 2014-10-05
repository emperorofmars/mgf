/*
**	Author:		Martin Schwarz
**	Name:		mgf_init.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_INIT
#define MGF_INIT

#include "mgf_include.h"

namespace mgf{

//###############################################################  	init class
class init{
public:
//###############################################################  constructor
	init(unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
					bool input_grabbed = 0, unsigned int monitor = 0);
	~init();
//###############################################################  setup
	bool add_window(unsigned int screen_w = 800, unsigned int screen_h = 600,
					bool fullscreen = 0, bool input_grabbed = 0, unsigned int monitor = 0);
	//bool setup_window(unsigned int window_num = 0, unsigned int screen_w = 800, unsigned int screen_h = 600,
	//				bool fullscreen = 0, bool input_grabbed = 0, unsigned int monitor = 0);
	void close_window(unsigned int window_num);
//###############################################################  init
	bool init_all();
//###############################################################  make current
	void current_window(unsigned int window_num);
//###############################################################  swap window
	void swap_window(unsigned int window_num);
	void swap_window();
//###############################################################  get
	float get_aspect_ratio(unsigned int window_num = 0);
private:
//###############################################################  window struct
	struct window{
		SDL_Window *window;
		unsigned int screen_w, screen_h, monitor;
		bool fullscreen, input_grabbed;
	};
//###############################################################  variables
	std::vector<struct window> mWindows;
	SDL_GLContext context;
	bool initialized;
	unsigned int renderer;
};

} // mgf

#endif
