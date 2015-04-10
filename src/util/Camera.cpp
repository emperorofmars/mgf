/*
**	Author:		Martin Schwarz
**	Name:		Camera.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Camera.h"

namespace mgf{

Camera::Camera(float fov, float screenratio, float near, float far, int mode, float mouse_speed, float move_speed){
	mProjection = glm::perspective(fov * (float)M_PI / 180, screenratio, near, far);
	mPos = glm::vec3(0, 0, 0);
	mRot = glm::vec3(0, 0, 0);
	mDir = glm::vec3(0, 0, -1);
	mUp = glm::vec3(0, 1, 0);
	mRight = glm::vec3(1, 0, 0);
	mView = glm::lookAt(mPos, mPos + mDir, mUp);
	mMode = mode;
	mMouseSpeed = mouse_speed;
	mMoveSpeed = move_speed;
	mFov = fov * M_PI / 180;
	mScreenratio = screenratio;
	mNear = near;
	mFar = far;
}

Camera::~Camera(){
}

glm::mat4 Camera::setup(float fov, float screenratio, float near, float far, int mode, float mouse_speed, float move_speed){
	mProjection = glm::perspective(fov * (float)M_PI / 180, screenratio, near, far);
	mMode = mode;
	mMouseSpeed = mouse_speed;
	mMoveSpeed = move_speed;
	mFov = fov * M_PI / 180;
	mScreenratio = screenratio;
	mNear = near;
	mFar = far;
	return mProjection * mView;
}

glm::mat4 Camera::setProjection(float fov, float screenratio, float near, float far){
	mProjection = glm::perspective(fov * (float)M_PI / 180, screenratio, near, far);
	mFov = fov * M_PI / 180;
	mScreenratio = screenratio;
	mNear = near;
	mFar = far;
	return mProjection * mView;
}

glm::mat4 Camera::setScreenratio(float screenratio){
	mProjection = glm::perspective(mFov, screenratio, mNear, mFar);
	mScreenratio = screenratio;
	return mProjection * mView;
}

void Camera::setSpeed(float mouse_speed, float move_speed){
	mMouseSpeed = mouse_speed;
	mMoveSpeed = move_speed;
	return;
}

void Camera::setMode(int mode){
	mMode = mode;
	mRot = glm::vec3(0);
	mDir = glm::vec3(0, 0, -1);
	mUp = glm::vec3(0, 1, 0);
	mRight = glm::vec3(1, 0, 0);
	return;
}

glm::mat4 Camera::update(glm::vec3 pos_, glm::vec3 rot_){
	int x = rot_[0];
	int y = rot_[1];
	//int z = rot_[2];

	if(mMode == 0){	//euler
		mRot[0] += (float)x * 0.008f * mMouseSpeed;
		mRot[1] -= (float)y * 0.008f * mMouseSpeed;

		if(mRot[0] > (float)M_PI) mRot[0] -= (float)M_PI * 2;
		if(mRot[0] < -(float)M_PI) mRot[0] += (float)M_PI * 2;

		if(mRot[1] > M_PI / 2 - 0.01) mRot[1] = M_PI / 2 - 0.01;
		if(mRot[1] < -M_PI / 2 + 0.01) mRot[1] = -M_PI / 2 + 0.01;

		mDir = glm::vec3(cos(mRot[0]) * cos(mRot[1]), sin(mRot[1]), sin(mRot[0]) * cos(mRot[1]));
		mRight = glm::cross(mDir, glm::vec3(0, 1, 0));
		mRight = glm::normalize(mRight);
	}
	else if(mMode == 1){	//quaternion
		mRot[0] = (float)x * -0.008f * mMouseSpeed;
		mRot[1] = (float)y * -0.008f * mMouseSpeed;

		mRight = glm::cross(mDir, mUp);
		glm::quat pitch = glm::angleAxis(mRot[1], mRight);
		pitch = glm::normalize(pitch);

		glm::quat heading = glm::angleAxis(mRot[0], mUp);
		heading = glm::normalize(heading);

		glm::quat temp = glm::cross(pitch, heading);
		temp = glm::normalize(temp);

		glm::mat3 dirmat = glm::mat3_cast(temp);
		mDir = dirmat * mDir;
		mDir = glm::normalize(mDir);

		mUp = dirmat * mUp;
		mUp = glm::normalize(mUp);
	}
	else if(mMode == 2){	//quaternion with fixed y axis
		mRot[0] = (float)x * -0.008f * mMouseSpeed;
		mRot[1] = (float)y * -0.008f * mMouseSpeed;

		mRight = glm::cross(mDir, mUp);
		glm::quat pitch = glm::angleAxis(mRot[1], mRight);
		pitch = glm::normalize(pitch);

		glm::quat heading = glm::angleAxis(mRot[0], glm::vec3(0, 1, 0));
		heading = glm::normalize(heading);

		glm::quat temp = glm::cross(pitch, heading);
		temp = glm::normalize(temp);

		glm::mat3 dirmat = glm::mat3_cast(temp);
		mDir = dirmat * mDir;
		mDir = glm::normalize(mDir);

		mRight = dirmat * mRight;
		mRight[1] = 0.f;
		mRight = glm::normalize(mRight);

		mUp = glm::cross(mRight, mDir);
		mUp = glm::normalize(mUp);
	}

	/*std::cerr << x << " " << y << " :: " << rot[0] << " " << rot[1] << std::endl;
	std::cerr << "dir: " << dir[0] << " " << dir[1] << " " << dir[2] << std::endl;
	std::cerr << "right: " << right[0] << " " << right[1] << " " << right[2] << std::endl << std::endl;*/

	mPos += mDir * pos_[0] * mMoveSpeed;
	mPos += mRight * pos_[1] * mMoveSpeed;

	mView = glm::lookAt(mPos, mPos + mDir, mUp);

	return mProjection * mView;
}

glm::mat4 Camera::getVP(){
	return mProjection * mView;
}

glm::mat4 Camera::getV(){
	return mView;
}

glm::mat4 Camera::getP(){
	return mProjection;
}


}












