/*
**	Author:		Martin Schwarz
**	Name:		mgf_model.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "mgf_model.h"

namespace mgf{


//###############################################################  obj_mesh class

//###############################################################  constructor
obj_mesh::obj_mesh(){
	material_index = 0;
	has_vertices = 0;
	has_normals = 0;
	has_texcoords = 0;
}

obj_mesh::~obj_mesh(){
	material_index = 0;
	has_vertices = 0;
	has_normals = 0;
	has_texcoords = 0;
}


//###############################################################  model class

//###############################################################  constructor
model::model(std::string file, GLuint uniform_trans, GLuint uniform_color){
	load_file(file);
	load_to_buffers();
	this->uniform_trans = uniform_trans;
	this->uniform_color = uniform_color;
	trans = glm::mat4(1.f);
}

model::~model(){
}


//###############################################################  load from file
bool model::load_file(std::string file){
	std::ifstream in(file);
	if(!in.is_open()){
		std::cerr << "failed to load file: " << file << std::endl;
		return false;
	}
	name = file;
	std::string line;
	obj_mesh *m = new obj_mesh;
	//std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> texcoords;
	std::vector<glm::ivec3> indices_normals;
	std::vector<glm::ivec3> indices_texcoords;

	while(getline(in, line)){
		if(line.substr(0, 2) == "v "){
			m->has_vertices = 1;
			glm::vec3 vertex(0);
			sscanf(line.c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			m->vertices.push_back(vertex);
			//std::cerr << "v " << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
		}
		else if(line.substr(0, 2) == "vn"){
			m->has_normals = 1;
			glm::vec3 normal(0);
			sscanf(line.c_str(), "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
			//std::cerr << "vn " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
			normals.push_back(normal);
		}
		else if(line.substr(0, 2) == "vt"){
			m->has_texcoords = 1;
			glm::vec3 texcoord(0);
			sscanf(line.c_str(), "vt %f %f", &texcoord[0], &texcoord[1]);
			//std::cerr << "vt " << texcoord[0] << " " << texcoord[1] << std::endl;
			texcoords.push_back(texcoord);
		}
		else if(line.substr(0, 2) == "f "){
			std::istringstream s(line.substr(2));
			glm::ivec3 vert_index(0), norm_index(0), texcoord_index(0);
			if(m->has_normals == 1 && m->has_texcoords == 1){
				sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&vert_index[0], &texcoord_index[0], &norm_index[0],
						&vert_index[1], &texcoord_index[1], &norm_index[1],
						&vert_index[2], &texcoord_index[2], &norm_index[2]);
				m->indices_vertex.push_back(vert_index);
				indices_normals.push_back(norm_index);
				indices_texcoords.push_back(texcoord_index);
			}
			else if(m->has_normals == 1 && m->has_texcoords == 0){
				sscanf(line.c_str(), "f %d/%d %d/%d %d/%d",
						&vert_index[0], &norm_index[0],
						&vert_index[1], &norm_index[1],
						&vert_index[2], &norm_index[2]);
				m->indices_vertex.push_back(vert_index);
				indices_normals.push_back(norm_index);
			}
			else if(m->has_normals == 0 && m->has_texcoords == 1){
				sscanf(line.c_str(), "f %d/%d %d/%d %d/%d",
						&vert_index[0], &texcoord_index[0],
						&vert_index[1], &texcoord_index[1],
						&vert_index[2], &texcoord_index[2]);
				m->indices_vertex.push_back(vert_index);
				indices_texcoords.push_back(texcoord_index);
			}
		}
		else if(line[0] == '#'){
			//ignore
		}
		else{
			//implement later
		}
	}

	for(unsigned int i = 0; i < m->indices_vertex.size(); i++){
		m->indices_vertex[i] -= 1;
	}
	if(m->has_normals){
		for(unsigned int i = 0; i < indices_normals.size(); i++){
			indices_normals[i] -= 1;
			m->normals.push_back(normals[indices_normals[i][0]]);
			m->normals.push_back(normals[indices_normals[i][1]]);
			m->normals.push_back(normals[indices_normals[i][2]]);
		}
	}
	if(m->has_texcoords){
		for(unsigned int i = 0; i < indices_texcoords.size(); i++){
			indices_texcoords[i] -= 1;
			m->texcoords.push_back(texcoords[indices_texcoords[i][0]]);
			m->texcoords.push_back(texcoords[indices_texcoords[i][1]]);
			m->texcoords.push_back(texcoords[indices_texcoords[i][2]]);
		}
	}

	/*for(unsigned int i = 0; i < m->vertices.size(); i++)
		std::cerr << "vertices: " << m->vertices[i][0] << " " << m->vertices[i][1] << " " << m->vertices[i][2] << std::endl;
	for(unsigned int i = 0; i < m->normals.size(); i++)
		std::cerr << "normals: " << m->normals[i][0] << " " << m->normals[i][1] << " " << m->normals[i][2] << std::endl;*/
	for(unsigned int i = 0; i < m->texcoords.size(); i++)
		std::cerr << "texcoords: " << m->texcoords[i][0] << " " << m->texcoords[i][1] << std::endl;

	meshes.push_back(m);

	std::cerr << "loaded successfully: " << file << std::endl;
	return true;
}

bool model::load_to_buffers(){
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[0]->indices_vertex.size() * sizeof(glm::ivec3), &meshes[0]->indices_vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, meshes[0]->vertices.size() * sizeof(glm::vec3), &meshes[0]->vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, meshes[0]->texcoords.size() * sizeof(glm::vec3), &meshes[0]->texcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	std::cerr << "loaded to buffers successfully: " << name << std::endl;
	return true;
}

//###############################################################  render
void model::render(){
	glUniformMatrix4fv(uniform_trans, 1, GL_FALSE, glm::value_ptr(trans));
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glUniform4fv(uniform_color, 1, glm::value_ptr(glm::vec4(0.5f, 0.5f, 0.5f, 1.f)));
	glDrawElements(GL_TRIANGLES, meshes[0]->indices_vertex.size() * sizeof(glm::ivec3), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, meshes[0]->vertices.size() * sizeof(glm::ivec3));
	return;
}


}
















