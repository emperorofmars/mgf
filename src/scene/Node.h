/*
**	Author:		Martin Schwarz
**	Name:		Node.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_NODE_H
#define MGF_NODE_H

#include "../Include.h"
#include "../data/Data.h"

namespace mgf{

class Node{
public:
	Node();
	virtual ~Node();

	std::shared_ptr<Node> find(const std::string &name);
	std::shared_ptr<Node> (unsigned int id);

	bool add(std::shared_ptr<Node> node);
	bool remove(const std::string mName);
	bool remove(unsigned int id);
	bool remove(std::shared_ptr<Node> node);

	virtual std::shared_ptr<Node> clone();

	void print(unsigned int deepness = 0);

private:
	std::string mName;
	unsigned int mID;
	static unsigned int mGlobalID;

	unsigned int mNumChildren;
	std::shared:ptr<Node> mParentNode;
	std::vector<std::shared:ptr<Node>> mChildNodes;

	friend class Loader;
};

} // mgf

#endif // MGF_NODE_H











