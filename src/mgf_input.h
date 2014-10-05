/*
**	Author:		Martin Schwarz
**	Name:		mgf_input.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_INPUT
#define MGF_INPUT

#include "mgf_include.h"

namespace mgf{

struct input_state{
	int move_dir, move_side, rotate_x, rotate_y, rotate_roll;
	std::vector<int> keys;
	bool quit, pause;
};

//###############################################################  	input class
class input{
public:
//###############################################################  constructor
	input();
	~input();
//###############################################################  setup
	void setup();
//###############################################################  update
	input_state update();
//###############################################################  get
	input_state get();
	glm::vec3 get_pos();
	glm::vec3 get_rot();
	bool get_quit();
protected:
//###############################################################  variables
	SDL_Event event;
	input_state state;
private:
};

}

#endif