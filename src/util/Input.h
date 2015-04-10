/*
**	Author:		Martin Schwarz
**	Name:		Input.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_INPUT_H
#define MGF_INPUT_H

#include "../Include.h"

namespace mgf{

struct InputState{
	int moveDir, moveSide, mouseX, mouseY;
	bool quit, pause;
};

class Input{
public:
	Input();
	~Input();

	int setup();

	InputState update();

	InputState get();
	glm::vec3 getPos();
	glm::vec3 getRot();
	bool getQuit();

private:
	SDL_Event mEvent;
	InputState mState;
};

} // mgf

#endif // MGF_INPUT_H
