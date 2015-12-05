/*
**	Author:		Martin Schwarz
**	Name:		LODNode.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "LODNode.h"

namespace mgf{

LODNode::LODNode(const std::string &name, float tresh1, float tresh2)
	: Node(name), mHighTresh(tresh1), mLowTresh(tresh2)
{
}

LODNode::~LODNode(){
}

std::shared_ptr<Node> LODNode::clone(){
	std::shared_ptr<LODNode> ret(new LODNode(count_up(mName)));

	mGlobalMutex.lock();
		ret->mID = mGlobalID;
		mGlobalID++;
	mGlobalMutex.unlock();

	mMutex.lock();
		ret->mName = mName;
		ret->mParentNode = mParentNode;

		ret->mTranslation = mTranslation;
		ret->mScale = mScale;
		ret->mTRS = mTRS;

		ret->mHigh = mHigh;
		ret->mMiddle = mMiddle;
		ret->mLow= mLow;
		
		ret->mHighTresh = mHighTresh;
		ret->mLowTresh = mLowTresh;

		for(auto iter = mChildNodesID.begin(); iter != mChildNodesID.end(); iter++){
			ret->add(iter->second->clone());
		}
	mMutex.unlock();

	return ret;
}

bool LODNode::update(std::shared_ptr<Renderer> renderer){
	renderer->clearLights();
	return updateImpl(glm::mat4(1), renderer);
}

bool LODNode::render(std::shared_ptr<Renderer> renderer){
	update(renderer);
	bool ret = renderImpl(glm::mat4(1), renderer);
	renderer->drawTransparent();
	return ret;
}

bool LODNode::updateImpl(glm::mat4 transform, std::shared_ptr<Renderer> renderer){
	if(!mVisible) return true;

	transform *= getTRS();
	for(auto iter = mChildNodesID.begin(); iter != mChildNodesID.end(); iter++){
		if(!iter->second->updateImpl(transform, renderer)){
			LOG_F_ERROR(MGF_LOG_FILE, "Rendering Failed!");
			return false;
		}
	}
	return true;
}

bool LODNode::renderImpl(glm::mat4 transform, std::shared_ptr<Renderer> renderer){
	if(!mVisible) return true;

	transform *= getTRS();
	
	glm::vec4 pos(getTranslation(), 1.f);
	glm::vec4 cam(renderer->getCamera()->getPos(), 1.f);
	pos = transform * pos;
	float distance = fabs(glm::distance(cam, pos));
	
	if(distance < mHighTresh) mHigh->renderImpl(transform, renderer);
	else if(distance < mLowTresh) mMiddle->renderImpl(transform, renderer);
	else mLow->renderImpl(transform, renderer);
	
	for(auto iter = mChildNodesID.begin(); iter != mChildNodesID.end(); iter++){
		if(!iter->second->renderImpl(transform, renderer)){
			return false;
		}
	}
	return true;
}

} // mgf



















