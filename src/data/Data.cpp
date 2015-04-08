/*
**	Author:		Martin Schwarz
**	Name:		Data.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Data.h"

namespace mgf{
namespace data{

//######################  data

Data::Data(){
}

Data::~Data(){
	_meshes.clear();
	_materials.clear();
	_textures.clear();
	_lights.clear();
}

Mesh::Mesh(){
	mIndexbuffer = 0;
	mVertexbuffer = 0;
	mNormalbuffer = 0;
	mVAO = 0;
	material_index = 0;
	render_indexed = true;
}

Mesh::~Mesh(){
	//std::cerr << "deleting vao: " << mVAO << std::endl;
	/*glBindVertexArray(mVAO);

	if(mIndexbuffer > 0) glDeleteBuffers(1, &mIndexbuffer);
	if(mVertexbuffer > 0) glDeleteBuffers(1, &mVertexbuffer);
	if(mNormalbuffer > 0) glDeleteBuffers(1, &mNormalbuffer);
	for(unsigned int i = 0; i < uvbuffer.size(); i++) glDeleteBuffers(1, &uvbuffer[i]);

	glDeleteVertexArrays(1, &mVAO);
	glBindVertexArray(0);*/
}

Material::Material(){
	diffuse[0] = 0.8f;
	diffuse[1] = 0.4f;
	diffuse[2] = 0.0f;
	diffuse[3] = 1.f;
}

Material::~Material(){
}

Texture::Texture(){
	texturebuffer = 0;
	image = NULL;
}

Texture::~Texture(){
	texturebuffer = 0;
	if(image != NULL) SDL_FreeSurface(image);
	image = NULL;
}

Light::Light(){
	strength = 1;
}

Light::~Light(){
}


} // data
} // mgf
















