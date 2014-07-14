

#ifndef MGF_LIBS
#define MGF_LIBS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "../../libs/mvm/mvm.hpp"

#endif

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


