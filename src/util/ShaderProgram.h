/*
**	Author:		Martin Schwarz
**	Name:		ShaderProgram.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_SHADER_H
#define MGF_SHADER_H

#include "../Include.h"

namespace mgf{

#define _shader_matrix_model "m_mat"
#define _shader_matrix_view_perspective "vp_mat"
#define _shader_material_color_diffuse "material.color"
#define _shader_material_alpha "material.alpha"
#define _shader_material_has_texture "material.has_texture"

enum shader_loc_enum{
	MATRIX_MODEL = 0,
	MATRIX_VP = 1,
	MATERIAL_COLOR_DIFFUSE = 2,
	MATERIAL_ALPHA = 3,
	MATERIAL_HAS_TEXTURE = 4,
	SIZE = 5
};

struct shader_to_programm{	//shader locations
	std::string path;
	GLenum type;
};

class ShaderProgram{
public:
	ShaderProgram();
	ShaderProgram(std::vector<struct shader_to_programm> &shaders);
	~ShaderProgram();

	void addShader(std::string sourcefile, GLenum shader_type);

	int createProgram();

	GLuint getProgram();
	GLuint get(shader_loc_enum loc);

	void use();

private:
	GLuint createShader(std::string &sourcefile, GLenum shader_type);
	void printShaderLog(GLuint shader);

	std::vector<struct shader_to_programm> mShaders;
	GLuint mProgram;
	GLuint mLocations[SIZE];
};

} // mgf

#endif // MGF_SHADER_H
