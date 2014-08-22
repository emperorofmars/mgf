/*
**	Author:		Martin Schwarz
**	Name:		mgf_camera.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_camera.h"

namespace mgf{

//###############################################################  	camera class

//###############################################################  constructor
camera::camera(float fov, float screenratio, float near, float far, float mouse_speed, float move_speed){
	projection = glm::perspective(fov, screenratio, near, far);
	pos = glm::vec3(0, 0, 0);
	rot = glm::vec3(0, 0, 0);
	dir = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
	view = glm::lookAt(pos, pos + dir, up);
	mode = 3;
	this->mouse_speed = mouse_speed;
	this->move_speed = move_speed;
}

camera::~camera(){
}
//###############################################################  setup
glm::mat4 camera::setup(float fov, float screenratio, float near, float far, float mouse_speed, float move_speed){
	projection = glm::perspective(fov, screenratio, near, far);
	return projection * view;
}

//###############################################################  update
glm::mat4 camera::update(glm::vec3 pos_, glm::vec3 rot_){
	int x = rot_[0];
	int y = rot_[1];
	int z = rot_[2];

	if(mode == 0){	//euler
		rot[0] += (float)x * 0.008f * mouse_speed;
		rot[1] -= (float)y * 0.008f * mouse_speed;

		if(rot[0] > (float)M_PI) rot[0] -= (float)M_PI * 2;
		if(rot[0] < -(float)M_PI) rot[0] += (float)M_PI * 2;

		if(rot[1] > M_PI / 2 - 0.01) rot[1] = M_PI / 2 - 0.01;
		if(rot[1] < -M_PI / 2 + 0.01) rot[1] = -M_PI / 2 + 0.01;

		dir = glm::vec3(cos(rot[0]) * cos(rot[1]), sin(rot[1]), sin(rot[0]) * cos(rot[1]));
		right = glm::cross(dir, glm::vec3(0, 1, 0));
		right = glm::normalize(right);
	}
	else if(mode >= 1 && mode <= 2){	//quaternion
		rot[0] = (float)x * -0.008f * mouse_speed;
		rot[1] = (float)y * -0.008f * mouse_speed;

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
	else if(mode == 3){	//quaternion with fixed y axis
		rot[0] = (float)x * -0.008f * mouse_speed;
		rot[1] = (float)y * -0.008f * mouse_speed;

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

	pos += dir * pos_[0] * move_speed;
	pos += right * pos_[1] * move_speed;

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













