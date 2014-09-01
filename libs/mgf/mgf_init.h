/*
**	Author:		Martin Schwarz
**	Name:		mgf_init.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_global.h"

namespace mgf{


//###############################################################  	init class
class init{
public:
//###############################################################  constructor
	init(unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
			bool input_grabbed = 0, unsigned int monitor = 0, unsigned int framerate = 240);
	~init();
//###############################################################  setup
	bool setup_window(unsigned int window_num = 0, unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
			bool input_grabbed = 0, unsigned int monitor = 0, unsigned int framerate = 240);
	unsigned int add_window();
//###############################################################  init
	bool init_all();
	bool init_window(unsigned int window_num);
//###############################################################  make current
	void current_window(unsigned int window_num);
//###############################################################  swap window
	void swap_window(unsigned int window_num);
	void swap_window();
//###############################################################  get
	float get_aspect_ratio(unsigned int window_num = 0);
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
