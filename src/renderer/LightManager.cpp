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

void LightManager::add(std::shared_ptr<Light> light){
	if(!light) return;
	mLights.push_back(light);
	assembleTexture();

	std::cerr << "LIGHT: " << light->mColor[0] << " " << light->mColor[1] << " " << light->mColor[2] << std::endl;


	return;
}

void LightManager::remove(std::shared_ptr<Light> light){
	for(unsigned int i = 0; i < mLights.size(); i++){
		if(mLights[i] == light){
			mLights.erase(mLights.begin() + i);
			break;
		}
	}
	assembleTexture();
	return;
}

void LightManager::remove(const std::string &name){
	for(unsigned int i = 0; i < mLights.size(); i++){
		if(mLights[i]->mName == name){
			mLights.erase(mLights.begin() + i);
			break;
		}
	}
	assembleTexture();
	return;
}

void LightManager::clear(){
	mLights.clear();
	mTexture.reset(new Texture());
	return;
}

void LightManager::refresh(){
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
	if(mLights.size() > 0){
		float lightTexture[mLights.size()][16];

		for(unsigned int i = 0; i < mLights.size(); i++){
			lightTexture[i][0] = mLights[i]->mActive;
			lightTexture[i][1] = mLights[i]->mType;
			lightTexture[i][2] = mLights[i]->mStrengthDiffuse;
			lightTexture[i][3] = mLights[i]->mStrengthSpecular;

			lightTexture[i][4] = mLights[i]->mColor[0];
			lightTexture[i][5] = mLights[i]->mColor[1];
			lightTexture[i][6] = mLights[i]->mColor[2];
			lightTexture[i][7] = 1;

			lightTexture[i][8] = mLights[i]->mPosition[0];
			lightTexture[i][9] = mLights[i]->mPosition[1];
			lightTexture[i][10] = mLights[i]->mPosition[2];
			lightTexture[i][11] = 1;

			lightTexture[i][12] = mLights[i]->mDirection[0];
			lightTexture[i][13] = mLights[i]->mDirection[1];
			lightTexture[i][14] = mLights[i]->mDirection[2];
			lightTexture[i][15] = 0;
		}

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &mTexture->mTextureBuffer);
		glBindTexture(GL_TEXTURE_2D, mTexture->mTextureBuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 16, mLights.size());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, mLights.size(), GL_RGBA, GL_FLOAT, lightTexture);
	}
	return;
}

} // mgf









