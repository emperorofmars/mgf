/*
**	Author:		Martin Schwarz
**	Name:		LoaderLOD.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_LODLOADER_H
#define MGF_LODLOADER_H

#include "../Include.h"
#include "Loader.h"
#include "LoaderTransparent.h"
#include "../data/Data.h"

namespace mgf{

class LoaderLOD{
public:
	virtual ~LoaderLOD();

	virtual std::shared_ptr<Node> load(const std::string &name, const std::string &high, const std::string &middle, const std::string &low, float tresh1 = 8, float tresh2 = 16, bool transparent = false);
};

} // mgf

#endif // MGF_LODLOADER_H












