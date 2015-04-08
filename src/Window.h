/*
**	Author:		Martin Schwarz
**	Name:		Window.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef WINDOW_H
#define WINDOW_H

#include "mgf_include.h"

namespace mgf{

//###############################################################  	Window class
class Window{
public:
	Window(unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
			unsigned int monitor = 0, bool input_grabbed = 0, bool vsync = 0);
	~Window();

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

#endif // WINDOW_H
