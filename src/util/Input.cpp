/*
**	Author:		Martin Schwarz
**	Name:		Input.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Input.h"

namespace mgf{

Input::Input(){
	SDL_SetRelativeMouseMode(SDL_TRUE);

	mState.moveDir = 0;
	mState.moveSide = 0;
	mState.mouseX = 0;
	mState.mouseY = 0;
	mState.quit = false;
	mState.pause = false;
}

Input::~Input(){
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

int Input::setup(){
	return 0;
}

InputState Input::update(){
	while(SDL_PollEvent(&mEvent) != 0){
		if(mEvent.type == SDL_QUIT) mState.quit = true;
		else{
			if(mEvent.type == SDL_KEYDOWN){
				switch(mEvent.key.keysym.sym){
				case SDLK_q:
					mState.quit = true;
					break;
				case SDLK_ESCAPE:
					if(SDL_GetRelativeMouseMode() == SDL_TRUE){
						SDL_SetRelativeMouseMode(SDL_FALSE);
						mState.pause = true;
					}
					else{
						SDL_SetRelativeMouseMode(SDL_TRUE);
						mState.pause = false;
					}
					break;
				}
			}
		}
	}

	SDL_GetRelativeMouseState(&mState.mouseX, &mState.mouseY);

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

	mState.moveDir = f - b;
	mState.moveSide = r - l;

	return mState;
}

InputState Input::get(){
	return mState;
}

glm::vec3 Input::getPos(){
	return glm::vec3(mState.moveDir, mState.moveSide, 0);
}

glm::vec3 Input::getRot(){
	return glm::vec3(mState.mouseY, mState.mouseX, 0);
}

bool Input::getQuit(){
	return mState.quit;
}

}



















