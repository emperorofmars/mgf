/*
**	Author:		Martin Schwarz
**	Name:		Label.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "Label.h"

namespace mgf{

Label::Label(const std::string &name)
	: Moveable(name)
{
	mTop.reset(new OverlayNode("Top"));
	mBase->add(mTop);
	mTop->translate(glm::vec3(0.f, 0.f, 1.f));
	mTop->scale(glm::vec3(0.6f, 0.6f, 0.6f));
	std::shared_ptr<Mesh> plane(createPlane());
	plane->mMaterial->mDiffuseColor = glm::vec4(0.1, 0.5, 0.5, 1);
	plane->mMaterial->mShadingType = 0;
	mTopMat = plane->mMaterial;
	mTop->addMesh(plane);
}

Label::~Label(){
}

bool Label::render(std::shared_ptr<Renderer> renderer){
	if(mBase)
		return mBase->render(renderer);
	else return false;
}

bool Label::setText(const std::string &text){
	mText = text;
	return textToTexture(mText);
}

bool Label::setBackground(const std::string &path){
	mBaseMat->mDiffuseTextures.resize(1);
	mBaseMat->mDiffuseTextures[0] = loadTextureFromPath(path);
	if(mBaseMat->mDiffuseTextures[0]) return true;
	return false;
}

bool Label::setBackground(std::shared_ptr<Texture> texture){
	mBaseMat->mDiffuseTextures.resize(1);
	mBaseMat->mDiffuseTextures[0] = texture;
	if(mBaseMat->mDiffuseTextures[0]) return true;
	return false;
}

std::string Label::getText(){
	return mText;
}

std::shared_ptr<Texture> Label::getBackground(){
	if(mBaseMat->mDiffuseTextures.size() > 0)
		return mBaseMat->mDiffuseTextures[0];
	return NULL;
}

bool Label::textToTexture(const std::string &text){
	return false;
}


} // mgf



















