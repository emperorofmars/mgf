/*
**	Author:		Martin Schwarz
**	Name:		mgf_scene.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_SCENE
#define MGF_SCENE

#include "mgf_include.h"

namespace mgf{

//###############################################################  scene class
class scene{
public:
//###############################################################  constructor
	scene();
	~scene();
//###############################################################  manage
	bool add(std::string path);
	bool remove(unsigned int index);
//###############################################################  variables
	std::vector<int> objects; //change
protected:
private:
};

}

#endif
