/*
**	Author:		Martin Schwarz
**	Name:		mgf.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_extra.h"

namespace mgf{

//###############################################################  stoT
template<> void stoT<float>(std::string str, std::size_t *pos, float *in){
	*in = std::stof(str, pos);
	return;
}

template<> void stoT<double>(std::string str, std::size_t *pos, double *in){
	*in = std::stod(str, pos);
	return;
}

} // mgf
