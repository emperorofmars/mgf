/*
**	Author:		Martin Schwarz
**	Name:		MeshTransparentNode.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_MESHTRANSPARENTNODE_H
#define MGF_MESHTRANSPARENTNODE_H

#include "Node.h"
#include "MeshNode.h"
#include "../data/Data.h"
#include "../camera/ICamera.h"

namespace mgf{

class MeshTransparentNode: public Node{
public:
	MeshTransparentNode(const std::string &name = "");
	~MeshTransparentNode();

	virtual std::shared_ptr<Node> clone();

	virtual bool addMesh(std::shared_ptr<Mesh> data);
	virtual bool removeMesh(std::shared_ptr<Mesh> data);

	virtual void setMaterial(std::shared_ptr<Material> material);
	virtual void resetMaterial();

	virtual bool update(std::shared_ptr<Renderer> renderer);
	virtual bool render(std::shared_ptr<Renderer> renderer);

protected:
	virtual bool updateImpl(glm::mat4 transform, std::shared_ptr<Renderer> renderer);
	virtual bool renderImpl(glm::mat4 transform, std::shared_ptr<Renderer> renderer);

	void calculateDistances();
	
	std::vector<std::shared_ptr<Mesh>> mMeshes;
	std::shared_ptr<Material> mMaterial;
	std::vector<glm::vec3> mPositions;
	std::vector<float> mDistances;

	friend class Loader;
	friend class LoaderTransparent;
};

} // mgf

#endif // MGF_MESHTRANSPARENTNODE_H











