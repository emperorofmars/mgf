/*
**	Author:		Martin Schwarz
**	Name:		MeshNode.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "MeshNode.h"

namespace mgf{

MeshNode::MeshNode(const std::string &name)
	: Node(name)
{
}

MeshNode::~MeshNode(){
}

std::shared_ptr<Node> MeshNode::clone(){
	std::shared_ptr<MeshNode> ret(new MeshNode(count_up(mName)));

	mGlobalMutex.lock();
		ret->mID = mGlobalID;
		mGlobalID++;
	mGlobalMutex.unlock();

	mMutex.lock();
		ret->mName = mName;
		ret->mParentNode = mParentNode;

		ret->mTranslation = mTranslation;
		ret->mRotation = mRotation;
		ret->mScale = mScale;
		ret->mTRS = mTRS;
		ret->mTRSCurrent = mTRSCurrent;

		ret->mMeshes = mMeshes;

		for(auto iter = mChildNodesID.begin(); iter != mChildNodesID.end(); iter++){
			ret->add(iter->second->clone());
		}
	mMutex.unlock();

	return ret;
}

bool MeshNode::addMesh(std::shared_ptr<Mesh>){
	return true;
}

bool MeshNode::removeMesh(std::shared_ptr<Mesh>){
	return true;
}

bool MeshNode::update(Renderer &renderer){
	return true;
}

bool MeshNode::render(Renderer &renderer){
	return true;
}

bool MeshNode::updateImpl(glm::mat4 transform, Renderer &renderer){
	return true;
}

bool MeshNode::renderImpl(glm::mat4 transform, Renderer &renderer){
	return true;
}

} // mgf



















