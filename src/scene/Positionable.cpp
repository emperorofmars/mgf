/*
**	Author:		Martin Schwarz
**	Name:		Positionable.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Positionable.h"

namespace mgf{

Positionable::Positionable(const std::string &name)
	: Node(name)
{
	mTRSCurrent = true;
}

Positionable::~Positionable(){
}

void Positionable::translate(glm::vec3 data){
	mTranslation += data;
}

void Positionable::rotate(float angle, glm::vec3 data){
	mRotation *= glm::rotate(glm::mat4(1), angle, data);
}

void Positionable::scale(glm::vec3 data){
	mScale *= data;
}

glm::mat4 Positionable::calculateTRS(){
	mTRS = glm::scale(glm::mat4(1), mScale);
	mTRS *= mRotation;
	mTRS *= glm::translate(glm::mat4(1), mTranslation);
	return mTRS;
}

void Positionable::setTranslation(glm::vec3 data){
	mTranslation = data;
}

void Positionable::setRotation(glm::mat4 data){
	mRotation = data;
}

void Positionable::setScale(glm::vec3 data){
	mScale = data;
}

glm::vec3 Positionable::getTranslation(){
	return mTranslation;
}

glm::mat4 Positionable::getRotation(){
	return mRotation;
}

glm::vec3 Positionable::getScale(){
	return mScale;
}

glm::mat4 Positionable::getTRS(){
	return mTRS;
}

glm::vec3 Positionable::getGlobalTranslation(){
	glm::vec3 ret;
	Positionable *tmp;
	if(mParentNode){
		tmp = dynamic_cast<Positionable *>(mParentNode);
		if(tmp){
			ret = tmp->getGlobalTranslation();
		}
	}
	return ret + mTranslation;
}

glm::mat4 Positionable::getGlobalRotation(){
	return mRotation;
}

glm::vec3 Positionable::getGlobalScale(){
	return mScale;
}

glm::mat4 Positionable::getGlobalTRS(){
	return mTRS;
}

std::shared_ptr<Node> Positionable::clone(){
	std::shared_ptr<Positionable> ret(new Positionable());

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

		for(auto iter = mChildNodesID.begin(); iter != mChildNodesID.end(); iter++){
			ret->add(iter->second->clone());
		}
	mMutex.unlock();

	return ret;
}

}



















