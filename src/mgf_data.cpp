/*
**	Author:		Martin Schwarz
**	Name:		mgf_data.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_data.h"

namespace mgf{

//######################  data

mgf_data::mgf_data(){
}

mgf_data::~mgf_data(){
	_meshes.clear();
	_materials.clear();
	_textures.clear();
	_lights.clear();
}

mgf_data::mgf_mesh::mgf_mesh(){
	elementbuffer = 0;
	vertexbuffer = 0;
	normalbuffer = 0;
	vao = 0;
	material_index = 0;
}

mgf_data::mgf_mesh::~mgf_mesh(){
	//std::cerr << "deleting vao: " << vao << std::endl;
	/*glBindVertexArray(vao);

	if(elementbuffer > 0) glDeleteBuffers(1, &elementbuffer);
	if(vertexbuffer > 0) glDeleteBuffers(1, &vertexbuffer);
	if(normalbuffer > 0) glDeleteBuffers(1, &normalbuffer);
	for(unsigned int i = 0; i < uvbuffer.size(); i++) glDeleteBuffers(1, &uvbuffer[i]);

	glDeleteVertexArrays(1, &vao);
	glBindVertexArray(0);*/
}

mgf_data::mgf_material::mgf_material(){
	diffuse[0] = 0.8f;
	diffuse[1] = 0.4f;
	diffuse[2] = 0.0f;
	diffuse[3] = 1.f;
}

mgf_data::mgf_material::~mgf_material(){
}

mgf_data::mgf_texture::mgf_texture(){
	texturebuffer = 0;
	image = NULL;
}

mgf_data::mgf_texture::~mgf_texture(){
	texturebuffer = 0;
	if(image != NULL) SDL_FreeSurface(image);
	image = NULL;
}

mgf_data::mgf_light::mgf_light(){
	strength = 1;
}

mgf_data::mgf_light::~mgf_light(){
}



}
















