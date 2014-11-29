/*
**	Author:		Martin Schwarz
**	Name:		mgf_window.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_window.h"

namespace mgf{

//###############################################################  window class


//###############################################################  constructor
window::window(unsigned int screen_w, unsigned int screen_h, bool fullscreen,
		   unsigned int monitor, bool input_grabbed, bool vsync)
{
	_inited = mgf_info::_mgf_inited;
	mgf_info::_windows.push_back(this);

	_window = NULL;
	_screen_w = screen_w;
	_screen_h = screen_h;
	_fullscreen = fullscreen;
	_input_grabbed = input_grabbed;
	_monitor = monitor;
	_vsync = vsync;
	if(_inited == true) init();
}

window::~window(){
	if(_inited == true){
		close();
		std::cerr << "window destroyed!" << std::endl;
	}
}

//###############################################################  setup window
bool window::init(){
	if(mgf_info::_mgf_inited == true) _inited = true;
	else return false;

	if(_inited == true){
		int flags = SDL_WINDOW_SHOWN;
		if(mgf_info::_renderer == OPENGL_3_3 || mgf_info::_renderer == OPENGL_4_3)
			flags |= SDL_WINDOW_OPENGL;
		if(_fullscreen == 1) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if(_input_grabbed == 1) flags |= SDL_WINDOW_INPUT_GRABBED;

		int windowpos_x = SDL_WINDOWPOS_UNDEFINED_DISPLAY(_monitor);
		int windowpos_y = SDL_WINDOWPOS_UNDEFINED_DISPLAY(_monitor);

		_window = SDL_CreateWindow("mgf", windowpos_x, windowpos_y,
							_screen_w, _screen_h, flags);
		if(_window == NULL){
			std::cerr << "SDL_CreateWindow failed! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}
		std::cerr << "window initialized successfully!" << std::endl;
	}
	else _window = NULL;
	return true;
}

void window::close(){
	if(_inited == true){
		SDL_DestroyWindow(_window);
		_window = NULL;
		_inited = false;
	}
	return;
}

void window::use(){
	if(_inited == true){
		SDL_GL_MakeCurrent(_window, mgf_info::_context);
		mgf_info::_aspect_ratio = get_aspect_ratio();
	}
	return;
}

void window::swap(){
	if(_inited == true) SDL_GL_SwapWindow(_window);
	return;
}

float window::get_aspect_ratio(){
	if(_screen_h != 0) return (float)_screen_w / (float)_screen_h;
	return 0;
}



//###############################################################  mgf_init
bool mgf_init(renderer_enum renderer){
	bool success = true;
	if(mgf_info::_mgf_inited != false) return false;
	if(mgf_info::_windows.size() == 0){
		std::cerr << "Cannot initialize, no window created!" << std::endl;
		return false;
	}

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
		mgf_info::_renderer = renderer;
		mgf_info::_mgf_inited = true;

		for(unsigned int i = 0; i < mgf_info::_windows.size(); i++){
			if(!mgf_info::_windows[i]->init()){
				std::cerr << "SDL_CreateWindow failed! SDL_Error: " << SDL_GetError() << std::endl;
				success = false;
			}
		}
		if(renderer == OPENGL_4_3){
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		}
		else if(renderer == OPENGL_3_3){
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		}
		else if(renderer == SOFTWARE){
			std::cerr << "Software renderer not yet supportet!" << std::endl;
			success = false;
		}
		else{
			std::cerr << "Invalid renderer!" << std::endl;
			success = false;
		}

		if(success){
			mgf_info::_context = SDL_GL_CreateContext(mgf_info::_windows[0]->_window);
			if(mgf_info::_context == NULL){
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

	if(!success) mgf_close();

	std::cerr << "SDL initialized successfully!" << std::endl;

	return true;
}

void mgf_close(){
	if(mgf_info::_mgf_inited == true){
		std::cerr << "initializing SDL failed!" << std::endl;
		std::cerr << "closing SDL" << std::endl;
		mgf_info::_mgf_inited = false;
		SDL_GL_DeleteContext(mgf_info::_context);
		mgf_info::_context = NULL;

		for(unsigned int i = 0; i < mgf_info::_windows.size(); i ++){
			mgf_info::_windows[i]->close();
		}
		mgf_info::_windows.clear();
		SDL_Quit();
		std::cerr << "SDL closed!" << std::endl;
	}
	else std::cerr << "Not inited!" << std::endl;
	return;
}

} // mgf










