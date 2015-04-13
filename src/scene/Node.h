/*
**	Author:		Martin Schwarz
**	Name:		Node.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_NODE_H
#define MGF_NODE_H

#include "../Include.h"

namespace mgf{

typedef unsigned int mgfID_t;

class Node{
public:
	Node();
	virtual ~Node();

	virtual std::shared_ptr<Node> clone();

	std::shared_ptr<Node> find(const std::string &name);
	std::shared_ptr<Node> getNode(mgfID_t id);

	bool add(std::shared_ptr<Node> node);
	bool remove(const std::string name);
	bool remove(unsigned int id);
	bool remove(std::shared_ptr<Node> node);

	void print(unsigned int deepness = 0);

private:
	std::string mName;
	mgfID_t mID;

	unsigned int mNumChildren;
	std::shared_ptr<Node> mParentNode;
	std::vector<std::shared_ptr<Node>> mChildNodes;

	static mgfID_t mGlobalID;
	static std::mutex mMutex;

	friend class Loader;
};

} // mgf

#endif // MGF_NODE_H











