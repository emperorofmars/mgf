
#include "mgf_init.h"

namespace mgf{

//###############################################################  mgf class

//###############################################################  constructor
mgf::mgf(unsigned int screen_w, unsigned int screen_h, bool fullscreen,
		   bool input_grabbed, unsigned int monitor, unsigned int framerate)
{
	unsigned int w = add_window();
	mWindows[w].window = NULL;
	mWindows[w].screen_w = screen_w;
	mWindows[w].screen_h = screen_h;
	mWindows[w].fullscreen = fullscreen;
	mWindows[w].input_grabbed = input_grabbed;
	mWindows[w].monitor = monitor;
	mWindows[w].framerate = framerate;
}

mgf::~mgf(){
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
bool mgf::setup_window(unsigned int window_num, unsigned int screen_w, unsigned int screen_h,
				  bool fullscreen, bool input_grabbed, unsigned int monitor, unsigned int framerate)
{
	if(window_num == mWindows.size()) return false;
	if(window_num == mWindows.size()) mWindows.resize(window_num + 1);
	mWindows[window_num].window = NULL;
	mWindows[window_num].screen_w = screen_w;
	mWindows[window_num].screen_h = screen_h;
	mWindows[window_num].fullscreen = fullscreen;
	mWindows[window_num].input_grabbed = input_grabbed;
	mWindows[window_num].monitor = monitor;
	mWindows[window_num].framerate = framerate;
	return true;
}

unsigned int mgf::add_window(){
	unsigned int s = mWindows.size();
	mWindows.resize(s + 1);
	return s;
}

//###############################################################  init_sdl
bool mgf::init(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cerr << "SDL_Init failed! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	for(unsigned int i = 0; i < mWindows.size(); i ++){
		mWindows[i].window = SDL_CreateWindow("mgf", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindows[i].screen_w, mWindows[i].screen_h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		if(mWindows[i].window == NULL){
			std::cerr << "SDL_CreateWindow failed! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}
	}

	context = SDL_GL_CreateContext(mWindows[0].window);
	if(context == NULL){
		std::cerr << "SDL_GL_CreateContext failed!" << std::endl;
		return false;
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if(glewError != GLEW_OK){
		std::cerr << "glewInit failed!" << std::endl;
		return false;
	}

	if(SDL_GL_SetSwapInterval(1) < 0){
		std::cerr << "SDL_GL_SetSwapInterval failed! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	std::cerr << "SDL initialized successfully!" << std::endl;
	return true;
}

bool mgf::init_window(unsigned int window_num){
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
void mgf::current_window(unsigned int window_num){
	SDL_GL_MakeCurrent(mWindows[window_num].window, context);
	return;
}

//###############################################################  swap window
void mgf::swap_window(unsigned int window_num){
	SDL_GL_SwapWindow(mWindows[window_num].window);
	return;
}

void mgf::swap_window(){
	for(unsigned int i = 0; i < mWindows.size(); i ++){
		SDL_GL_SwapWindow(mWindows[i].window);
	}
	return;
}

void mgf::close_window(unsigned int window_num){
	SDL_DestroyWindow(mWindows[window_num].window);
	mWindows[window_num].window = NULL;
	mWindows.erase(mWindows.begin() + window_num);
	return;
}

} // mgf










