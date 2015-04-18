/*
**	Author:		Martin Schwarz
**	Name:		Helper.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Helper.h"

namespace mgf{

std::string vec3_toStr(glm::vec3 data){
	std::stringstream res;
	res << "( " << data[0] << ", " << data[1] << ", " << data[2] << " )";
	return res.str();
}

std::string vec4_toStr(glm::vec4 data){
	std::stringstream res;
	res << "( " << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << " )";
	return res.str();
}

std::string mat4_toStr(glm::mat4 data){
	std::stringstream res;
	res << "( " << data[0][0] << ", " << data[1][0] << ", " << data[2][0] << ", " << data[3][0] << " )" << std::endl;
	res << "( " << data[0][1] << ", " << data[1][1] << ", " << data[2][1] << ", " << data[3][1] << " )" << std::endl;
	res << "( " << data[0][2] << ", " << data[1][2] << ", " << data[2][2] << ", " << data[3][2] << " )" << std::endl;
	res << "( " << data[0][3] << ", " << data[1][3] << ", " << data[2][3] << ", " << data[3][3] << " )" << std::endl;
	return res.str();
}

} // mgf
