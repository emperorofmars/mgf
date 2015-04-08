/*
**	Author:		Martin Schwarz
**	Name:		Light.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_LIGHT_H
#define MGF_LIGHT_H

#include "../Include.h"

namespace mgf{
namespace data{

class Light{
public:
	Light();
	~Light();

	std::string mName;

	glm::vec3 mDiffuse;
	float mStrength;
};

} // data
} // mgf

#endif // MGF_LIGHT_H








