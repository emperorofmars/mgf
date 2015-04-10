/*
**	Author:		Martin Schwarz
**	Name:		Camera.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_CAMERA_H
#define MGF_CAMERA_H

#include "../Include.h"

namespace mgf{

class Camera{
public:
	Camera(float fov = 90.f, float screenratio = 1.f, float near = 0.1f,
			float far = 1000.f, int mode = 0, float mouse_speed = 1.f, float move_speed = 1.f);
	~Camera();

	glm::mat4 setup(float fov = 90.f, float screenratio = 1.f, float near = 0.1f,
					float far = 1000.f, int mode = 0, float mouse_speed = 1.f, float move_speed = 1.f);
	glm::mat4 setProjection(float fov = 90.f, float screenratio = 1.f, float near = 0.1f, float far = 1000.f);
	glm::mat4 setScreenratio(float screenratio = 1.f);

	void setSpeed(float mouse_speed = 1.f, float move_speed = 1.f);
	void setMode(int mode = 0);

	glm::mat4 update(glm::vec3 pos_, glm::vec3 rot_);

	glm::mat4 getVP();
	glm::mat4 getV();
	glm::mat4 getP();

private:
	glm::mat4 mProjection, mView;
	glm::vec3 mPos, mRot, mDir, mUp, mRight;
	unsigned int mMode;
	float mMouseSpeed, mMoveSpeed;
	float mFov, mScreenratio, mNear, mFar;
};

} // mgf

#endif // MGF_CAMERA_H
