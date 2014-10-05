/*
**	Author:		Martin Schwarz
**	Name:		mgf_init.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_init.h"

namespace mgf{

//###############################################################  init class

//###############################################################  constructor
init::init(unsigned int screen_w, unsigned int screen_h, bool fullscreen,
		   bool input_grabbed, unsigned int monitor)
{
	unsigned int w = add_window();
	mWindows[w].window = NULL;
	mWindows[w].screen_w = screen_w;
	mWindows[w].screen_h = screen_h;
	mWindows[w].fullscreen = fullscreen;
	mWindows[w].input_grabbed = input_grabbed;
	mWindows[w].monitor = monitor;
}

init::~init(){
	std::cerr << "closing SDL" << std::endl;
	SDL_GL_DeleteContext(context);
	context = NULL;
	for(unsigned int i = 0; i < mWindows.size(); i ++){
		SDL_DestroyWindow(mWindows[i].window);
		mWindows[i].window = NULL;
	}
	mWindows.clear();
	SDL_Quit();
	std::cerr << "SDL closed!" << std::endl;
}

//###############################################################  setup window
bool init::setup_window(unsigned int window_num, unsigned int screen_w, unsigned int screen_h,
						bool fullscreen, bool input_grabbed, unsigned int monitor)
{
	bool create = false;
	if(window_num > mWindows.size()) return false;
	if(window_num == mWindows.size()){
		mWindows.resize(window_num + 1);
		create = true;
	}
	mWindows[window_num].window = NULL;
	mWindows[window_num].screen_w = screen_w;
	mWindows[window_num].screen_h = screen_h;
	mWindows[window_num].fullscreen = fullscreen;
	mWindows[window_num].input_grabbed = input_grabbed;
	mWindows[window_num].monitor = monitor;
	if(create == true) init::init_window(window_num);
	return true;
}

unsigned int init::add_window(){
	unsigned int s = mWindows.size();
	mWindows.resize(s + 1);
	return s;
}

//###############################################################  init_sdl
bool init::init_all(){
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cerr << "SDL_Init failed! SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	if(success){
		int flags = IMG_INIT_PNG | IMG_INIT_JPG;
		if(!(IMG_Init(flags) & flags))
		{
			std::cerr << "IMG_Init failed! IMG_Error: " << IMG_GetError() << std::endl;
			success = false;
		}
	}
	if(success){
		for(unsigned int i = 0; i < mWindows.size(); i ++){
			int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
			if(mWindows[i].fullscreen == 1) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			if(mWindows[i].input_grabbed == 1) flags |= SDL_WINDOW_INPUT_GRABBED;

			int windowpos_x = SDL_WINDOWPOS_UNDEFINED_DISPLAY(mWindows[i].monitor);
			int windowpos_y = SDL_WINDOWPOS_UNDEFINED_DISPLAY(mWindows[i].monitor);

			mWindows[i].window = SDL_CreateWindow("mgf", windowpos_x, windowpos_y, mWindows[i].screen_w, mWindows[i].screen_h, flags);
			if(mWindows[i].window == NULL){
				std::cerr << "SDL_CreateWindow failed! SDL_Error: " << SDL_GetError() << std::endl;
				success = false;
			}
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		if(success){
			context = SDL_GL_CreateContext(mWindows[0].window);
			if(context == NULL){
				std::cerr << "SDL_GL_CreateContext failed!" << std::endl;
				success = false;
			}
		}
		if(success){
			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if(glewError != GLEW_OK){
				std::cerr << "glewInit failed!" << std::endl;
				success = false;
			}
		}

		if(success){
			if(SDL_GL_SetSwapInterval(1) < 0){
				std::cerr << "SDL_GL_SetSwapInterval failed! SDL_Error: " << SDL_GetError() << std::endl;
				success = false;
			}
		}
	}

	if(!success){
		std::cerr << "initializing SDL failed!" << std::endl;
		std::cerr << "closing SDL" << std::endl;
		SDL_GL_DeleteContext(context);
		context = NULL;
		for(unsigned int i = 0; i < mWindows.size(); i ++){
			SDL_DestroyWindow(mWindows[i].window);
			mWindows[i].window = NULL;
		}
		mWindows.clear();
		SDL_Quit();
		std::cerr << "SDL closed!" << std::endl;
	}

	std::cerr << "SDL initialized successfully!" << std::endl;
	return true;
}

bool init::init_window(unsigned int window_num){
	mWindows[window_num].window = SDL_CreateWindow("mgf", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindows[window_num].screen_w,
												   mWindows[window_num].screen_h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if(mWindows[window_num].window == NULL){
		std::cerr << "SDL_CreateWindow failed! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	std::cerr << "window " << window_num << " initialized successfully!" << std::endl;
	return true;
}

//###############################################################  make current
void init::current_window(unsigned int window_num){
	SDL_GL_MakeCurrent(mWindows[window_num].window, context);
	return;
}

//###############################################################  swap window
void init::swap_window(unsigned int window_num){
	SDL_GL_SwapWindow(mWindows[window_num].window);
	return;
}

void init::swap_window(){
	for(unsigned int i = 0; i < mWindows.size(); i ++){
		SDL_GL_SwapWindow(mWindows[i].window);
	}
	return;
}

//###############################################################  get
float init::get_aspect_ratio(unsigned int window_num){
	return (float)mWindows[window_num].screen_w / (float)mWindows[window_num].screen_h;
}

void init::close_window(unsigned int window_num){
	SDL_DestroyWindow(mWindows[window_num].window);
	mWindows[window_num].window = NULL;
	mWindows.erase(mWindows.begin() + window_num);
	return;
}

} // mgf










