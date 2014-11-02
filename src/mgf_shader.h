/*
**	Author:		Martin Schwarz
**	Name:		mgf_shader.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_SHADER
#define MGF_SHADER

#include "mgf_include.h"

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
	GLuint get_m_mat();
	GLuint get_vp_mat();
//###############################################################  use
	void use();
protected:
private:
	GLuint create_shader(std::string &sourcefile, GLenum shader_type);
	void printShaderLog(GLuint shader);
//###############################################################  variables
	std::vector<struct shader_to_programm> mShaders;
	GLuint mProgram;
	GLuint m_mat, vp_mat;
};

} // mgf

#endif
