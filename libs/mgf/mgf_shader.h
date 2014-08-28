/*
**	Author:		Martin Schwarz
**	Name:		mgf_shader.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_global.h"

namespace mgf{

struct shader_to_programm{	//shader locations
	std::string path;
	GLenum type;
};

//###############################################################  shader program class
class shader_program{
public:
//###############################################################  constructor
	shader_program();
	shader_program(std::vector<struct shader_to_programm> &shaders);
	~shader_program();
//###############################################################  add shader
	void add_shader(std::string sourcefile, GLenum shader_type);
//###############################################################  create
	bool create_program();
//###############################################################  use
	GLuint get_program();
//###############################################################  use
	void use();
protected:
private:
	GLuint create_shader(std::string &sourcefile, GLenum shader_type);
	void printShaderLog(GLuint shader);
//###############################################################  variables
	std::vector<struct shader_to_programm> mShaders;
	GLuint mProgram;
};

} // mgf


