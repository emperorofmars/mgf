/*
**	Author:		Martin Schwarz
**	Name:		ILoader.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_ILOADER_H
#define MGF_ILOADER_H

#include "../Include.h"
#include "MeshNode.h"
#include "MeshTransparentNode.h"
#include "../data/Data.h"

namespace mgf{

class ILoader{
public:
	virtual ~ILoader(){};

	virtual std::shared_ptr<Node> load(const std::string &file) = 0;
	virtual void clear() = 0;
};

} // mgf

#endif // MGF_ILOADER_H












