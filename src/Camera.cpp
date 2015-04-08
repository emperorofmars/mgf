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
camera::camera(float fov, float screenratio, float near, float far, int mode, float mouse_speed, float move_speed){
	_projection = glm::perspective(fov * (float)M_PI / 180, screenratio, near, far);
	_pos = glm::vec3(0, 0, 0);
	_rot = glm::vec3(0, 0, 0);
	_dir = glm::vec3(0, 0, -1);
	_up = glm::vec3(0, 1, 0);
	_right = glm::vec3(1, 0, 0);
	_view = glm::lookAt(_pos, _pos + _dir, _up);
	_mode = mode;
	_mouse_speed = mouse_speed;
	_move_speed = move_speed;
	_fov = fov * M_PI / 180;
	_screenratio = screenratio;
	_near = near;
	_far = far;
}

camera::~camera(){
}

//###############################################################  setup
glm::mat4 camera::setup(float fov, float screenratio, float near, float far, int mode, float mouse_speed, float move_speed){
	_projection = glm::perspective(fov * (float)M_PI / 180, screenratio, near, far);
	_mode = mode;
	_mouse_speed = mouse_speed;
	_move_speed = move_speed;
	_fov = fov * M_PI / 180;
	_screenratio = screenratio;
	_near = near;
	_far = far;
	return _projection * _view;
}

glm::mat4 camera::set_projection(float fov, float screenratio, float near, float far){
	_projection = glm::perspective(fov * (float)M_PI / 180, screenratio, near, far);
	_fov = fov * M_PI / 180;
	_screenratio = screenratio;
	_near = near;
	_far = far;
	return _projection * _view;
}

glm::mat4 camera::set_screenratio(float screenratio){
	_projection = glm::perspective(_fov, screenratio, _near, _far);
	_screenratio = screenratio;
	return _projection * _view;
}

void camera::set_speed(float mouse_speed, float move_speed){
	_mouse_speed = mouse_speed;
	_move_speed = move_speed;
	return;
}

void camera::set_mode(int mode){
	_mode = mode;
	_rot = glm::vec3(0);
	_dir = glm::vec3(0, 0, -1);
	_up = glm::vec3(0, 1, 0);
	_right = glm::vec3(1, 0, 0);
	return;
}

//###############################################################  update
glm::mat4 camera::update(glm::vec3 pos_, glm::vec3 rot_){
	int x = rot_[0];
	int y = rot_[1];
	//int z = rot_[2];

	if(_mode == 0){	//euler
		_rot[0] += (float)x * 0.008f * _mouse_speed;
		_rot[1] -= (float)y * 0.008f * _mouse_speed;

		if(_rot[0] > (float)M_PI) _rot[0] -= (float)M_PI * 2;
		if(_rot[0] < -(float)M_PI) _rot[0] += (float)M_PI * 2;

		if(_rot[1] > M_PI / 2 - 0.01) _rot[1] = M_PI / 2 - 0.01;
		if(_rot[1] < -M_PI / 2 + 0.01) _rot[1] = -M_PI / 2 + 0.01;

		_dir = glm::vec3(cos(_rot[0]) * cos(_rot[1]), sin(_rot[1]), sin(_rot[0]) * cos(_rot[1]));
		_right = glm::cross(_dir, glm::vec3(0, 1, 0));
		_right = glm::normalize(_right);
	}
	else if(_mode == 1){	//quaternion
		_rot[0] = (float)x * -0.008f * _mouse_speed;
		_rot[1] = (float)y * -0.008f * _mouse_speed;

		_right = glm::cross(_dir, _up);
		glm::quat pitch = glm::angleAxis(_rot[1], _right);
		pitch = glm::normalize(pitch);

		glm::quat heading = glm::angleAxis(_rot[0], _up);
		heading = glm::normalize(heading);

		glm::quat temp = glm::cross(pitch, heading);
		temp = glm::normalize(temp);

		glm::mat3 dirmat = glm::mat3_cast(temp);
		_dir = dirmat * _dir;
		_dir = glm::normalize(_dir);

		_up = dirmat * _up;
		_up = glm::normalize(_up);
	}
	else if(_mode == 2){	//quaternion with fixed y axis
		_rot[0] = (float)x * -0.008f * _mouse_speed;
		_rot[1] = (float)y * -0.008f * _mouse_speed;

		_right = glm::cross(_dir, _up);
		glm::quat pitch = glm::angleAxis(_rot[1], _right);
		pitch = glm::normalize(pitch);

		glm::quat heading = glm::angleAxis(_rot[0], glm::vec3(0, 1, 0));
		heading = glm::normalize(heading);

		glm::quat temp = glm::cross(pitch, heading);
		temp = glm::normalize(temp);

		glm::mat3 dirmat = glm::mat3_cast(temp);
		_dir = dirmat * _dir;
		_dir = glm::normalize(_dir);

		_right = dirmat * _right;
		_right[1] = 0.f;
		_right = glm::normalize(_right);

		_up = glm::cross(_right, _dir);
		_up = glm::normalize(_up);
	}

	/*std::cerr << x << " " << y << " :: " << rot[0] << " " << rot[1] << std::endl;
	std::cerr << "dir: " << dir[0] << " " << dir[1] << " " << dir[2] << std::endl;
	std::cerr << "right: " << right[0] << " " << right[1] << " " << right[2] << std::endl << std::endl;*/

	_pos += _dir * pos_[0] * _move_speed;
	_pos += _right * pos_[1] * _move_speed;

	_view = glm::lookAt(_pos, _pos + _dir, _up);

	return _projection * _view;
}

//###############################################################  get
glm::mat4 camera::get_vp(){
	return _projection * _view;
}

glm::mat4 camera::get_v(){
	return _view;
}

glm::mat4 camera::get_p(){
	return _projection;
}


}












