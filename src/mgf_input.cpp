/*
**	Author:		Martin Schwarz
**	Name:		mgf_input.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_input.h"

namespace mgf{


//###############################################################  input class

//###############################################################  constructor
input::input(){
	state.move_dir = 0;
	state.move_side = 0;
	state.rotate_roll = 0;
	state.rotate_x = 0;
	state.rotate_y = 0;
	state.quit = 0;
	state.pause = 0;
}

input::~input(){
}

void input::setup(){
	return;
}

input_state input::update(){
	while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT) state.quit = true;
			else{
				if(event.type == SDL_KEYDOWN){
					switch(event.key.keysym.sym){
					case SDLK_q:
						state.quit = true;
						break;
					case SDLK_ESCAPE:
						if(SDL_GetRelativeMouseMode() == SDL_TRUE){
							SDL_SetRelativeMouseMode(SDL_FALSE);
							state.pause = true;
						}
						else{
							SDL_SetRelativeMouseMode(SDL_TRUE);
							state.pause = false;
						}
						break;
					}
				}
			}
		}

	SDL_GetRelativeMouseState(&state.rotate_x, &state.rotate_y);

	const Uint8 *key = SDL_GetKeyboardState(NULL);
	int f = 0, b = 0, l = 0, r = 0;

	if(key[SDL_SCANCODE_W]){
		f = 1;
	}
	if(key[SDL_SCANCODE_S]){
		b = 1;
	}
	if(key[SDL_SCANCODE_D]){
		r = 1;
	}
	if(key[SDL_SCANCODE_A]){
		l = 1;
	}

	state.move_dir = f - b;
	state.move_side = r - l;

	return state;
}

input_state input::get(){
	return state;
}

glm::vec3 input::get_pos(){
	return glm::vec3(state.move_dir, state.move_side, 0);
}

glm::vec3 input::get_rot(){
	return glm::vec3(state.rotate_x, state.rotate_y, state.rotate_roll);
}

bool input::get_quit(){
	return state.quit;
}

}



















