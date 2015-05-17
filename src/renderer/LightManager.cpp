/*
**	Author:		Martin Schwarz
**	Name:		LightManager.cpp
**	Project:	mgf - Mars Graphics Framework
*/

#include "LightManager.h"

namespace mgf{

LightManager::LightManager(){
	mTexture.reset(new Texture());
}

LightManager::~LightManager(){
}

void LightManager::addLight(std::shared_ptr<Light> light){
	if(!light) return;
	mLights.push_back(light);
	assembleTexture();
	return;
}

void LightManager::removeLight(std::shared_ptr<Light> light){
	for(unsigned int i = 0; i < mLights.size(); i++){
		if(mLights[i] == light){
			mLights.erase(mLights.begin() + i);
			break;
		}
	}
	assembleTexture();
	return;
}

void LightManager::removeLight(const std::string &name){
	for(unsigned int i = 0; i < mLights.size(); i++){
		if(mLights[i]->mName == name){
			mLights.erase(mLights.begin() + i);
			break;
		}
	}
	assembleTexture();
	return;
}

unsigned int LightManager::getHeight(){
	return mLights.size();
}

std::shared_ptr<Light> LightManager::getLight(const std::string &name){
	for(unsigned int i = 0; i < mLights.size(); i++){
		if(mLights[i]->mName == name){
			return mLights[i];
		}
	}
	return NULL;
}

std::vector<std::shared_ptr<Light>> LightManager::getLights(){
	return mLights;
}

std::shared_ptr<Texture> LightManager::getTexture(){
	return mTexture;
}

void LightManager::assembleTexture(){
	mTexture.reset(new Texture());
	float lightTexture[mLights.size()][6];

	for(unsigned int i = 0; i < mLights.size(); i++){
		lightTexture[i][0] = mLights[i]->mActive;
		lightTexture[i][1] = mLights[i]->mStrengthDiffuse;
		lightTexture[i][2] = mLights[i]->mStrengthSpecular;
		lightTexture[i][3] = 0;
		lightTexture[i][4] = mLights[i]->mColor[0];
		lightTexture[i][5] = mLights[i]->mColor[1];
		lightTexture[i][6] = mLights[i]->mColor[2];
		lightTexture[i][7] = 0;
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &mTexture->mTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, mTexture->mTextureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 8, mLights.size());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, mLights.size(), GL_RGBA, GL_FLOAT, lightTexture);
	glGenerateMipmap(GL_TEXTURE_2D);

	return;
}

} // mgf









