/*
**	Author:		Martin Schwarz
**	Name:		LoaderLOD.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "LoaderLOD.h"
#include "Loader.h"
#include "LoaderTransparent.h"
#include "LODNode.h"

namespace mgf{

LoaderLOD::~LoaderLOD(){
	return;
}

std::shared_ptr<Node> LoaderLOD::load(const std::string &name, const std::string &high, const std::string &middle, const std::string &low, float tresh1, float tresh2, bool transparent){
	std::unique_ptr<ILoader> loader;
	if(transparent) loader.reset(new LoaderTransparent());
	else loader.reset(new Loader());
	
	auto ret = std::make_shared<LODNode>(name);
	
	ret->mHigh = loader->load(high);
	ret->mMiddle = loader->load(middle);
	ret->mLow = loader->load(low);
	
	if(!ret->mHigh || !ret->mMiddle|| !ret->mLow) return NULL;
	
	return ret;
}


} // mgf



















