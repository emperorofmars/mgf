/*
**	Author:		Martin Schwarz
**	Name:		Texture.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_TEXTURE_H
#define MGF_TEXTURE_H

#include "../Include.h"

namespace mgf{
namespace data{

class Texture{
public:
	Texture();
	~Texture();

	std::string mName;

	GLuint texturebuffer;
	SDL_Surface *mImage;
};

} // data
} // mgf

#endif // MGF_TEXTURE_H








