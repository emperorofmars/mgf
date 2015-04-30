/*
**	Author:		Martin Schwarz
**	Name:		CameraFPSEuler.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "CameraFPSEuler.h"

namespace mgf{

CameraFPSEuler::CameraFPSEuler(float fov, float screenratio, float far, float near)
    : BaseCamera(fov, screenratio, far, near){}

CameraFPSEuler::~CameraFPSEuler(){
}



glm::mat4 Camera::update(glm::vec3 pos, glm::vec3 rot){
	int x = rot[1];
	int y = rot[0];
	//int z = rot_[2];

    mRot[0] += (float)x * 0.008f * mMouseSpeed;
    mRot[1] -= (float)y * 0.008f * mMouseSpeed;

    if(mRot[0] > (float)M_PI) mRot[0] -= (float)M_PI * 2;
    if(mRot[0] < -(float)M_PI) mRot[0] += (float)M_PI * 2;

    if(mRot[1] > M_PI / 2 - 0.01) mRot[1] = M_PI / 2 - 0.01;
    if(mRot[1] < -M_PI / 2 + 0.01) mRot[1] = -M_PI / 2 + 0.01;

    mDir = glm::vec3(cos(mRot[0]) * cos(mRot[1]), sin(mRot[1]), sin(mRot[0]) * cos(mRot[1]));
    mRight = glm::cross(mDir, glm::vec3(0, 1, 0));
    mRight = glm::normalize(mRight);

	mPos += mDir * pos[0] * mMoveSpeed;
	mPos += mRight * pos[1] * mMoveSpeed;

	mView = glm::lookAt(mPos, mPos + mDir, mUp);

	return mProjection * mView;
}

} // mgf












