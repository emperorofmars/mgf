

#ifndef MGF_LIBS
#define MGF_LIBS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "../../libs/mvm/mvm.hpp"

#endif

namespace mgf{

struct window{	//window info
	SDL_Window *window;
	unsigned int screen_w, screen_h, monitor, framerate;
	bool fullscreen, input_grabbed;
};

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
protected:
private:
//###############################################################  variables
	std::vector<struct window> mWindows;
	SDL_GLContext context;
	SDL_Event event;
};

} // mgf
