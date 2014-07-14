/*
**	Author:		Martin Schwarz
**	Name:		mol.hpp
**	Project:	mol - Mars OBJ Loader
**	Compile:	-std=c++11 // include in other project
**	License:	WTFPL
*/

/*
**	TODO:
**		stuff
*/

#ifndef MOL
#define MOL

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "../../libs/mvm/mvm.hpp"

namespace mol{

//###############################################################  mesh class
template<typename T> class mesh{
public:
//###############################################################  constructor
	mesh();
	mesh(std::string file);
	~mesh();
//###############################################################  load from file
	bool load_file(std::string file);
//###############################################################  get stuff
	mvm::vecN<T, 4>* get_vertices();
	unsigned int get_size();
	unsigned int get_num();
protected:
//###############################################################  variables
	std::vector<mvm::vecN<T, 4>> vert;
	std::vector<mvm::vecN<T, 3>> norm;
	std::vector<unsigned int> vert_index;
	std::vector<unsigned int> norm_index;
private:
	bool vn;
};

#include "mol.inl"

} // mol


#endif // MOL






