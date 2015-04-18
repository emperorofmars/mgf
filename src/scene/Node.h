/*
**	Author:		Martin Schwarz
**	Name:		Node.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_NODE_H
#define MGF_NODE_H

#include "../Include.h"
#include "Positionable.h"
#include "../renderer/Renderer.h"

namespace mgf{

typedef unsigned int mgfID_t;

class Node : public Positionable{
public:
	Node(const std::string &name = "");
	virtual ~Node();

	virtual std::shared_ptr<Node> clone();

	std::shared_ptr<Node> find(const std::string &name);
	std::shared_ptr<Node> getChild(const std::string &name);

	bool add(std::shared_ptr<Node> node);
	bool remove(const std::string name);
	bool remove(unsigned int id);

	virtual bool update(Renderer &renderer);
	virtual bool render(Renderer &renderer);

	mgfID_t getID();
	std::string getName();

	glm::vec3 getGlobalTranslation();
	glm::mat4 getGlobalRotation();
	glm::vec3 getGlobalScale();
	glm::mat4 getGlobalTRS();

	void print(unsigned int deepness = 0);

protected:
	virtual bool updateImpl(glm::mat4 transform, Renderer &renderer);
	virtual bool renderImpl(glm::mat4 transform, Renderer &renderer);

	std::string mName;
	mgfID_t mID;

	std::mutex mMutex;

	unsigned int mNumChildren;
	Node *mParentNode;
	std::unordered_map<mgfID_t, std::shared_ptr<Node>> mChildNodesID;
	std::unordered_map<std::string, std::shared_ptr<Node>> mChildNodesString;

	static mgfID_t mGlobalID;
	static std::mutex mGlobalMutex;

	friend class Loader;
};

} // mgf

#endif // MGF_NODE_H











