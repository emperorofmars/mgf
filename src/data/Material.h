/*
**	Author:		Martin Schwarz
**	Name:		Material.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_MATERIAL_H
#define MGF_MATERIAL_H

#include "../Include.h"
#include "Texture.h"

namespace mgf{

class Material{
public:
	Material();
	~Material();

	std::string mName;

	std::vector<std::shared_ptr<Texture>> mDiffuseTexture;
	glm::vec4 mDiffuseColor;
};

} // mgf

#endif // MGF_MATERIAL_H








