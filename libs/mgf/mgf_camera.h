/*
**	Author:		Martin Schwarz
**	Name:		mgf_camera.h
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

//###############################################################  	camera class
class camera{
public:
//###############################################################  constructor
	camera(float fov = 90.f, float screenratio = 1.f, float near = 0.1f, float far = 1000.f, float mouse_speed = 1.f, float move_speed = 1.f);
	~camera();
//###############################################################  setup
	glm::mat4 setup(float fov = 90.f, float screenratio = 1.f, float near = 0.1f, float far = 1000.f, float mouse_speed = 1.f, float move_speed = 1.f);
//###############################################################  update
	glm::mat4 update(glm::vec3 pos_, glm::vec3 rot_);
//###############################################################  get
	glm::mat4 get_vp();
	glm::mat4 get_v();
	glm::mat4 get_p();
//###############################################################  variables
	glm::mat4 projection, view;
	glm::vec3 pos, rot, dir, up, right;
	unsigned int mode;
	float mouse_speed, move_speed;
protected:
private:
};

}
