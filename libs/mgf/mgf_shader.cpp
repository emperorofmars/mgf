/*
**	Author:		Martin Schwarz
**	Name:		mgf.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_shader.h"

namespace mgf{

//###############################################################  shader_program class

//###############################################################  constructor
shader_program::shader_program(){
	mProgram = 0;
}

shader_program::shader_program(std::vector<struct shader_to_programm> &shaders){
	mProgram = 0;
	mShaders = shaders;
}

shader_program::~shader_program(){
	std::cerr << "closing shader_program" << std::endl;
	if(mProgram != 0) glDeleteProgram(mProgram);
	mProgram = 0;
	mShaders.clear();
}

//###############################################################  add shader
void shader_program::add_shader(std::string sourcefile, GLenum shader_type){
	struct shader_to_programm s = {sourcefile, shader_type};
	mShaders.push_back(s);
	return;
}

//###############################################################  get program
GLuint shader_program::get_program(){
	return mProgram;
}

//###############################################################  create
bool shader_program::create_program(){
    std::vector<GLuint> s;
	mProgram = 0;

    for(unsigned int i = 0; i < mShaders.size(); i++){
		s.push_back(create_shader(mShaders[i].path, mShaders[i].type));
    }
    mProgram = glCreateProgram();
    for(unsigned int i = 0; i < s.size(); i++){
		glAttachShader(mProgram, s[i]);
    }
    glLinkProgram(mProgram);
    GLint compilesuccess = GL_FALSE;
    glGetProgramiv(mProgram,GL_LINK_STATUS, &compilesuccess);
    if(compilesuccess == GL_TRUE){
        std::cerr << "Program: " << mProgram << " compiled successfully" << std::endl;
    }
    else{
        std::cerr << "Program: " << mProgram << " failed to compile!" << std::endl;
        printShaderLog(mProgram);
		glDeleteProgram(mProgram);
        mProgram = 0;
    }
    for(unsigned int i = 0; i < s.size(); i++){
		glDeleteShader(s[i]);
    }
    s.clear();
    mShaders.clear();

	if(mProgram == 0){
		std::cerr << "Creating program failed" << std::endl;
		return false;
	}
	else return true;
}

GLuint shader_program::create_shader(std::string &sourcefile, GLenum shader_type){
    GLuint shader = 0;
    std::ifstream input(sourcefile);
    std::string shader_string;
    if(input.is_open()){
        input.seekg(0, std::ios::end);
        shader_string.resize(input.tellg());
        input.seekg(0, std::ios::beg);
        input.read(&shader_string[0], shader_string.size());
        input.close();
    }
    else{
        std::cerr << "Shader: " << sourcefile << " could not be opened!" << std::endl;
        return shader;
    }
    //std::cout << shader_string << std::endl;
    shader = glCreateShader(shader_type);
    const GLchar *shader_code = shader_string.c_str();
    //std::cerr << shader_code << std::endl;
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    GLint compilesuccess = GL_FALSE;
    glGetShaderiv(shader,GL_COMPILE_STATUS, &compilesuccess);
    if(compilesuccess == GL_TRUE){
        std::cerr << "Shader: " << sourcefile << " compiled successfully" << std::endl;
    }
    else{
        std::cerr << "Shader: " << sourcefile << " failed to compile!" << std::endl;
        printShaderLog(shader);
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

void shader_program::printShaderLog(GLuint shader){
	GLint log_length = 0;
	if(glIsShader(shader))
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	else if(glIsProgram(shader))
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	else{
		std::cerr << "printlog: Not a shader or a program" << std::endl;
		return;
	}

	char log[log_length];

	if(glIsShader(shader))
		glGetShaderInfoLog(shader, log_length, NULL, (GLchar *)log);
	else if(glIsProgram(shader))
		glGetProgramInfoLog(shader, log_length, NULL, (GLchar *)log);

	std::cerr << log << std::endl;
	return;
}

//###############################################################  use
void shader_program::use(){
	glUseProgram(mProgram);
	return;
}

} // mgf










