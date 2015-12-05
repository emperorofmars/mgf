/*
**	Author:		Martin Schwarz
**	Name:		LODNode.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_LODNODE_H
#define MGF_LODNODE_H

#include "Node.h"
#include "../data/Data.h"

namespace mgf{

class LODNode: public Node{
public:
	LODNode(const std::string &name = "", float tresh1 = 8, float tresh2 = 16);
	~LODNode();

	virtual std::shared_ptr<Node> clone();

	virtual bool update(std::shared_ptr<Renderer> renderer);
	virtual bool render(std::shared_ptr<Renderer> renderer);

protected:
	virtual bool updateImpl(glm::mat4 transform, std::shared_ptr<Renderer> renderer);
	virtual bool renderImpl(glm::mat4 transform, std::shared_ptr<Renderer> renderer);

	std::shared_ptr<Node> mHigh;
	std::shared_ptr<Node> mMiddle;
	std::shared_ptr<Node> mLow;
	float mHighTresh;
	float mLowTresh;

	friend class Loader;
	friend class LoaderTransparent;
	friend class LoaderLOD;
};

} // mgf

#endif // MGF_LODNODE_H











