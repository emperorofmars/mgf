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
	Window(const std::string &name, unsigned int w = 800, unsigned int h = 600, bool fullscreen = 0,
			unsigned int monitor = 0, bool inputGrabbed = 0, bool vsync = 0);
	~Window();

	//int setup(unsigned int w = 800, unsigned int h = 600, bool fullscreen = 0,
	//				unsigned int monitor = 0, bool inputGrabbed = 0, bool vsync = 0);
	int open(const std::string &name);
	int close();

	int use();
	int swap();
	int blit(std::shared_ptr<SDL_Surface> image);
	float getAspectRatio();

private:
	int initSDL();
	int closeSDL();

	SDL_Window *mWindow;
	unsigned int mW, mH, mMonitor;
	bool mFullscreen, mInputGrabbed, mVsync;
	bool mOpened;

	static SDL_GLContext *mContext;

	static bool mSDLInited;
	static unsigned int mNumWindows;
};


} // mgf

#endif // WINDOW_H
