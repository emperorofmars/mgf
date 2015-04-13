/*
**	Author:		Martin Schwarz
**	Name:		Mesh.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_MESH_H
#define MGF_MESH_H

#include "../Include.h"

#include "Texture.h"

namespace mgf{

class Mesh{
public:
	Mesh();
	~Mesh();

	std::string mName;

	std::shared_ptr<Texture> mMaterial;
	unsigned int mNumIndices, mNumVertices, mNumNormals;

	bool mRenderIndexed;

	std::vector<unsigned int> mIndices;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<std::vector<glm::vec3>> mUV;

	GLuint mVAO;
	GLuint mIndexbuffer, mVertexbuffer, mNormalbuffer;
	std::vector<GLuint> mUVBuffer;
};

} // mgf

#endif // MGF_MESH_H








