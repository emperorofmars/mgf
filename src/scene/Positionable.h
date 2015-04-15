/*
**	Author:		Martin Schwarz
**	Name:		Positionable.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_POSITIONABLE_H
#define MGF_POSITIONABLE_H

#include "Node.h"

namespace mgf{

class Positionable: public Node{
public:
	Positionable(const std::string &name = "");
	virtual ~Positionable();

	void translate(glm::vec3 data);
	void rotate(float angle, glm::vec3 data);
	void scale(glm::vec3 data);

	glm::mat4 calculateTRS();

	void setTranslation(glm::vec3 data);
	void setRotation(glm::mat4 data);
	void setScale(glm::vec3 data);

	glm::vec3 getTranslation();
	glm::mat4 getRotation();
	glm::vec3 getScale();
	glm::mat4 getTRS();

	glm::vec3 getGlobalTranslation();
	glm::mat4 getGlobalRotation();
	glm::vec3 getGlobalScale();
	glm::mat4 getGlobalTRS();

	virtual std::shared_ptr<Node> clone();

private:
	glm::vec3 mTranslation, mScale;
	glm::mat4 mTRS, mRotation;
	bool mTRSCurrent;
};

} // mgf

#endif // MGF_POSITIONABLE_H











