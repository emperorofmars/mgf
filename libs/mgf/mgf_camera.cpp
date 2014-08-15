/*
**	Author:		Martin Schwarz
**	Name:		mgf.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_camera.h"

namespace mgf{

//###############################################################  	camera class

//###############################################################  constructor
camera::camera(float fov, float screenratio, float near, float far){
	projection = glm::perspective(fov, screenratio, near, far);
	pos = glm::vec3(0, 0, 0);
	rot = glm::vec3(-M_PI / 2, 0, 0);
	view = glm::lookAt(pos, pos + rot, glm::vec3(0, 1, 0));
}

camera::~camera(){
}
//###############################################################  setup
glm::mat4 camera::setup(float fov, float screenratio, float near, float far){
	projection = glm::perspective(fov, screenratio, near, far);
}

//###############################################################  update
glm::mat4 camera::update(glm::vec3 pos_, glm::vec3 rot_){
	int x = 0, y = 0;
	SDL_GetMouseState(&x, &y);
	rot[0] += (float)(x - 400) * 0.008f;
	rot[1] -= (float)(y - 400) * 0.008f;

	if(rot[0] > (float)M_PI) rot[0] -= (float)M_PI * 2;
	if(rot[0] < -(float)M_PI) rot[0] += (float)M_PI * 2;

	if(rot[1] > M_PI / 2 - 0.01) rot[1] = M_PI / 2 - 0.01;
	if(rot[1] < -M_PI / 2 + 0.01) rot[1] = -M_PI / 2 + 0.01;

	glm::vec3 dir = glm::vec3(cos(rot[0]) * cos(rot[1]), sin(rot[1]), sin(rot[0]) * cos(rot[1]));
	glm::vec3 right = glm::cross(dir, glm::vec3(0, 1, 0));
	right = glm::normalize(right);
	view = glm::lookAt(pos, pos + dir, glm::vec3(0, 1, 0));

	std::cerr << x << " " << y << " :: " << rot[0] << " " << rot[1] << std::endl;
	std::cerr << "dir: " << dir[0] << " " << dir[1] << " " << dir[2] << std::endl;
	std::cerr << "right: " << right[0] << " " << right[1] << " " << right[2] << std::endl << std::endl;

	const Uint8 *key = SDL_GetKeyboardState(NULL);
	if(key[SDL_SCANCODE_W]){
		pos += dir * 0.6f;
	}
	if(key[SDL_SCANCODE_S]){
		pos -= dir * 0.6f;
	}
	if(key[SDL_SCANCODE_D]){
		pos += right * 0.6f;
	}
	if(key[SDL_SCANCODE_A]){
		pos -= right * 0.6f;
	}

	return projection * view;
}

//###############################################################  get
glm::mat4 camera::get_vp(){
	return projection * view;
}

glm::mat4 camera::get_v(){
	return view;
}

glm::mat4 camera::get_p(){
	return projection;
}


}













