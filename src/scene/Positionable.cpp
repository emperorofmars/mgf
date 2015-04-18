/*
**	Author:		Martin Schwarz
**	Name:		Positionable.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Positionable.h"

#include "../helper/Helper.h"

namespace mgf{

Positionable::Positionable(){
	mTRSCurrent = true;
	mTRS = glm::mat4(1);
	mRotation = glm::mat4(1);
	mScale = glm::vec3(1.f, 1.f, 1.f);
}

Positionable::~Positionable(){
}

void Positionable::translate(glm::vec3 data){
	mTranslation += data;
	mTRSCurrent = false;
}

void Positionable::rotate(float angle, glm::vec3 data){
	mRotation = glm::rotate(glm::mat4(1), angle, data) * mRotation;
	mTRSCurrent = false;
}

void Positionable::scale(glm::vec3 data){
	mScale *= data;
	mTRSCurrent = false;
}

glm::mat4 Positionable::calculateTRS(){
	mTRS = glm::scale(glm::mat4(1), mScale) * glm::mat4(1);
	mTRS = mRotation * mTRS;
	mTRS = glm::translate(glm::mat4(1), mTranslation) * mTRS;
	mTRSCurrent = true;
	return mTRS;
}

void Positionable::setTranslation(glm::vec3 data){
	mTranslation = data;
	mTRSCurrent = false;
}

void Positionable::setRotation(glm::mat4 data){
	mRotation = data;
	mTRSCurrent = false;
}

void Positionable::setScale(glm::vec3 data){
	mScale = data;
	mTRSCurrent = false;
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
	if(!mTRSCurrent){
		calculateTRS();
	}
	return mTRS;
}

}



















