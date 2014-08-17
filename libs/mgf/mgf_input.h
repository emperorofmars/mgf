/*
**	Author:		Martin Schwarz
**	Name:		mgf_input.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_LIBS
#define MGF_LIBS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/type_ptr.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"

#endif

namespace mgf{

struct input_state{
	int move_dir, move_side, rotate_x, rotate_y, rotate_roll;
	std::vector<int> keys;
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
protected:
//###############################################################  variables
	SDL_Event event;
	input_state state;
private:
};

}
