/*
**	Author:		Martin Schwarz
**	Name:		MeshNode.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_MESHNODE_H
#define MGF_MESHNODE_H

#include "Node.h"
#include "../data/Data.h"

namespace mgf{

class MeshNode: public Node{
public:
	MeshNode(const std::string &name = "");
	~MeshNode();

	virtual std::shared_ptr<Node> clone();

	bool addMesh(std::shared_ptr<Mesh>);
	bool removeMesh(std::shared_ptr<Mesh>);

	virtual bool update(Renderer &renderer);
	virtual bool render(Renderer &renderer);

private:
	virtual bool updateImpl(glm::mat4 transform, Renderer &renderer);
	virtual bool renderImpl(glm::mat4 transform, Renderer &renderer);

	std::vector<std::shared_ptr<Mesh>> mMeshes;
};

} // mgf

#endif // MGF_MESHNODE_H











