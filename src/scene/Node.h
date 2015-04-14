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
	std::shared_ptr<Node> getChild(const std::string &name);

	bool add(std::shared_ptr<Node> node);
	bool remove(const std::string name);
	bool remove(unsigned int id);

	mgfID_t getID();
	std::string getName();

	void print(unsigned int deepness = 0);

private:
	std::string mName;
	mgfID_t mID;

	std::mutex mMutex;

	unsigned int mNumChildren;
	std::shared_ptr<Node> mParentNode;
	std::unordered_map<mgfID_t, std::shared_ptr<Node>> mChildNodesID;
	std::unordered_map<std::string, std::shared_ptr<Node>> mChildNodesString;

	static mgfID_t mGlobalID;
	static std::mutex mGlobalMutex;

	friend class Loader;
};

} // mgf

#endif // MGF_NODE_H











