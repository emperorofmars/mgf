/*
**	Author:		Martin Schwarz
**	Name:		mgf_extra.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_global.h"

namespace mgf{

//###############################################################  convert string to float or double
template<typename T> void stoT(std::string str, std::size_t *pos, T *in);

} // mgf
