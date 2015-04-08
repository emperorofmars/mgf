/*
**	Author:		Martin Schwarz
**	Name:		mgf_node.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_NODE
#define MGF_NODE

#include "mgf_include.h"
#include "mgf_data.h"

namespace mgf{

class mgf_node;
class mgf_node_model;
class mgf_node_model_instance;

class mgf_node{
public:
	mgf_node();
	virtual ~mgf_node();

	mgf_node *find_node(std::string name);
	mgf_node *find_node(unsigned int id);
	mgf_node *get_by_path(std::string path);

	bool add(mgf_node *node);
	bool remove(std::string name);
	bool remove_by_path(std::string path);

	void print(unsigned int deepness = 0);

	virtual void render() = 0;

	std::string _name;
	unsigned int _id;
	static unsigned int _global_id;

	unsigned int _num_children;
	mgf_node *_parent_node;
	std::vector<mgf_node *> _child_nodes;
};

class mgf_node_mat{	//add recursive get_pos();
public:
	void translate(glm::vec3 data);
	void rotate(float angle, glm::vec3 data);
	void scale(glm::vec3 data);
	void multiply_mat(glm::mat4 data);

	glm::vec3 get_pos();

	glm::mat4 _trans;
};

class mgf_node_model: public mgf_node, public mgf_node_mat{
public:
	mgf_node_model();
	~mgf_node_model();

	void construct_from_ainode(aiNode *ainode, mgf_data *data, unsigned int oldsize_meshes);
	void construct_from_mgf_node(mgf_node_model *node);

	mgf_node_model_instance *create_instance();

	virtual void render();

	mgf_data *_data;
	std::vector<unsigned int> _meshes;

	glm::vec3 bbox_vol;
	glm::vec3 bbox_hur;

	unsigned int _num_meshes;
	unsigned int _num_instances;
	bool _render;
};

class mgf_node_model_instance: public mgf_node, public mgf_node_mat{
public:
	mgf_node_model_instance();
	~mgf_node_model_instance();

	void construct_from_mgf_node(mgf_node_model *node);

	virtual void render();

	unsigned int _model_id;
	mgf_node_model *_model;
	bool _render;
};

}

#endif











