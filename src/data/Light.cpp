/*
**	Author:		Martin Schwarz
**	Name:		Light.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Light.h"

namespace mgf{

Light::Light(){
	mStrengthDiffuse = 1;
	mStrengthSpecular = 1;
	mColor = glm::vec3(1.f, 1.f, 1.f);
}

Light::~Light(){
}

} // mgf
















