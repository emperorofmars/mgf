/*
**	Author:		Martin Schwarz
**	Name:		Light.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_LIGHT_H
#define MGF_LIGHT_H

#include "../Include.h"

namespace mgf{

class Light{
public:
	Light();
	~Light();

	std::string mName;

	glm::vec3 mDiffuse;
	float mStrength;
};

} // mgf

#endif // MGF_LIGHT_H








