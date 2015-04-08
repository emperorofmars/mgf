/*
**	Author:		Martin Schwarz
**	Name:		mgf_camera.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_CAMERA
#define MGF_CAMERA

#include "mgf_include.h"

namespace mgf{

//###############################################################  	camera class
class camera{
public:
	camera(float fov = 90.f, float screenratio = 1.f, float near = 0.1f, float far = 1000.f, int mode = 0, float mouse_speed = 1.f, float move_speed = 1.f);
	~camera();

	glm::mat4 setup(float fov = 90.f, float screenratio = 1.f, float near = 0.1f, float far = 1000.f, int mode = 0, float mouse_speed = 1.f, float move_speed = 1.f);
	glm::mat4 set_projection(float fov = 90.f, float screenratio = 1.f, float near = 0.1f, float far = 1000.f);
	glm::mat4 set_screenratio(float screenratio = 1.f);

	void set_speed(float mouse_speed = 1.f, float move_speed = 1.f);
	void set_mode(int mode = 0);

	glm::mat4 update(glm::vec3 pos_, glm::vec3 rot_);

	glm::mat4 get_vp();
	glm::mat4 get_v();
	glm::mat4 get_p();

private:
	glm::mat4 _projection, _view;
	glm::vec3 _pos, _rot, _dir, _up, _right;
	unsigned int _mode;
	float _mouse_speed, _move_speed;
	float _fov, _screenratio, _near, _far;
};

}

#endif
