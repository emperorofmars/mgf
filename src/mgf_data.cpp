/*
**	Author:		Martin Schwarz
**	Name:		mgf_data.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_data.h"

namespace mgf{

//######################  data

mgf_data::mgf_mesh::mgf_mesh(){
	elementbuffer = 0;
	vertexbuffer = 0;
	normalbuffer = 0;
	uvbuffer = NULL;
	vao = 0;
	material_index = 0;
}

}


