/*
**	Author:		Martin Schwarz
**	Name:		mgf_shader.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_SHADER
#define MGF_SHADER

#define _shader_matrix_model "m_mat"
#define _shader_matrix_view_perspective "vp_mat"
#define _shader_material_color_diffuse "material.color"
#define _shader_material_alpha "material.alpha"
#define _shader_material_has_texture "material.has_texture"

enum shader_loc{
	MATRIX_MODEL = 0,
	MATRIX_VP = 1,
	MATERIAL_COLOR_DIFFUSE = 2,
	MATERIAL_ALPHA = 3,
	MATERIAL_HAS_TEXTURE = 4,
	SIZE = 5
};

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
	GLuint get(shader_loc loc);
//###############################################################  use
	void use();
protected:
private:
	GLuint create_shader(std::string &sourcefile, GLenum shader_type);
	void printShaderLog(GLuint shader);
//###############################################################  variables
	std::vector<struct shader_to_programm> mShaders;
	GLuint mProgram;
	GLuint mLocations[SIZE];
};

} // mgf

#endif
