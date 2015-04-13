/*
**	Author:		Martin Schwarz
**	Name:		Renderable.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_RENDERABLE_H
#define MGF_RENDERABLE_H

#include "Positionable.h"
#include "../data/Data.h"

namespace mgf{

class Renderable: public Positionable{
public:
	Renderable();
	~Renderable();

	int setVisible(bool visible);

	virtual std::shared_ptr<Node> clone();

private:
	std::vector<std::shared_ptr<Mesh>> mMeshes;
	bool mVisible;
};

} // mgf

#endif // MGF_RENDERABLE_H











