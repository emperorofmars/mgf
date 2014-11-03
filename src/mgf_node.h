/*
**	Author:		Martin Schwarz
**	Name:		mgf_node.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_NODE
#define MGF_NODE

#include "mgf_include.h"

namespace mgf{

class mgf_node{
public:
	mgf_node();
	~mgf_node();

	void construct_from_ainode(aiNode *ainode);

	mgf_node *find_node(std::string name);

	void render();

	std::string _name;

	mgf_node *_parent_node;
	std::vector<mgf_node *> _child_nodes;
	std::vector<unsigned int> _meshes;

	unsigned int _num_children;
	unsigned int _num_meshes;
	unsigned int _num_instances;

	glm::mat4 _trans;

	bool _render;
};

}

#endif











