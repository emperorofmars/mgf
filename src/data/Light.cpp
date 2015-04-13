/*
**	Author:		Martin Schwarz
**	Name:		Light.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Light.h"

namespace mgf{

Light::Light(){
	mStrength = 1;
	mDiffuse = glm::vec3(1.f, 1.f, 1.f);
}

Light::~Light(){
}

} // mgf
















