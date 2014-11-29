/*
**	Author:		Martin Schwarz
**	Name:		mgf_init.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_INIT
#define MGF_INIT

#include "mgf_include.h"

enum renderer_enum{
	OPENGL_4_3 = 0,
	OPENGL_3_3 = 1,
	SOFTWARE = 2,
};

namespace mgf{

//###############################################################  	init class
class init{
public:
//###############################################################  constructor
	init();
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
	bool init_all(renderer_enum use_renderer);
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
	static std::vector<struct window> mWindows;
	static SDL_GLContext context;
	static bool initialized;
	static renderer_enum renderer;
};

} // mgf

#endif
