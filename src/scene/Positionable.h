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
	void translate(glm::vec3 data);
	void rotate(float angle, glm::vec3 data);
	void scale(glm::vec3 data);

	glm::mat4 calculateTRS();

	glm::vec3 getTranslation();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::mat4 getTRS();

	glm::vec3 getGlobalTranslation();
	glm::vec3 getGlobalRotation();
	glm::vec3 getGlobalScale();
	glm::mat4 getGlobalTRS();

	virtual std::shared_ptr<Node> clone();

private:
	glm::vec3 mTranslation, mRototation, mScale;
	glm::mat4 mTRS;
};

} // mgf

#endif // MGF_POSITIONABLE_H











