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
	rot = glm::vec3(0, 0, 0);
	dir = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
	view = glm::lookAt(pos, pos + dir, up);
	mode = 3;
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

	if(mode == 0){	//euler
		rot[0] += (float)(x - 400) * 0.008f;
		rot[1] -= (float)(y - 400) * 0.008f;

		if(rot[0] > (float)M_PI) rot[0] -= (float)M_PI * 2;
		if(rot[0] < -(float)M_PI) rot[0] += (float)M_PI * 2;

		if(rot[1] > M_PI / 2 - 0.01) rot[1] = M_PI / 2 - 0.01;
		if(rot[1] < -M_PI / 2 + 0.01) rot[1] = -M_PI / 2 + 0.01;

		dir = glm::vec3(cos(rot[0]) * cos(rot[1]), sin(rot[1]), sin(rot[0]) * cos(rot[1]));
		right = glm::cross(dir, glm::vec3(0, 1, 0));
		right = glm::normalize(right);
	}
	else if(mode >= 1 && mode <= 2){	//quaternion
		rot[0] = (float)(x - 400) * -0.5f;
		rot[1] = (float)(y - 400) * -0.5f;

		right = glm::cross(dir, up);
		glm::quat pitch = glm::angleAxis(rot[1], right);
		pitch = glm::normalize(pitch);

		glm::quat heading = glm::angleAxis(rot[0], up);
		heading = glm::normalize(heading);

		glm::quat temp = glm::cross(pitch, heading);
		temp = glm::normalize(temp);

		glm::mat3 dirmat = glm::mat3_cast(temp);
		dir = dirmat * dir;
		dir = glm::normalize(dir);

		if(mode >= 2){
			up = dirmat * up;
			up = glm::normalize(up);
		}
	}
	else if(mode == 3){	//quaternion
		rot[0] = (float)(x - 400) * -0.5f;
		rot[1] = (float)(y - 400) * -0.5f;

		right = glm::cross(dir, up);
		glm::quat pitch = glm::angleAxis(rot[1], right);
		pitch = glm::normalize(pitch);

		glm::quat heading = glm::angleAxis(rot[0], glm::vec3(0, 1, 0));

		heading = glm::normalize(heading);

		glm::quat temp = glm::cross(pitch, heading);
		temp = glm::normalize(temp);

		glm::mat3 dirmat = glm::mat3_cast(temp);
		dir = dirmat * dir;
		dir = glm::normalize(dir);

		right = dirmat * right;
		right[1] = 0;
		right = glm::normalize(right);

		up = glm::cross(right, dir);
		up = glm::normalize(up);
	}

	/*std::cerr << x << " " << y << " :: " << rot[0] << " " << rot[1] << std::endl;
	std::cerr << "dir: " << dir[0] << " " << dir[1] << " " << dir[2] << std::endl;
	std::cerr << "right: " << right[0] << " " << right[1] << " " << right[2] << std::endl << std::endl;*/

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

	view = glm::lookAt(pos, pos + dir, up);

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













