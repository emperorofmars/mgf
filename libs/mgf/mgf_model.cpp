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
	vertices.clear();
	texcoords.clear();
	normals.clear();
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
	std::cerr << "loading file: " << file << std::endl;
	std::ifstream in(file);
	if(!in.is_open()){
		std::cerr << "failed to load file: " << file << std::endl;
		return false;
	}
	name = file;
	std::string line;
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec3> tmp_texcoords;
	std::vector<glm::vec3> tmp_normals;
	std::vector<std::vector<unsigned int>> indices_vertex;
	std::vector<std::vector<unsigned int>> indices_texcoords;
	std::vector<std::vector<unsigned int>> indices_normals;
	std::vector<bool> has_vertices;
	std::vector<bool> has_texcoords;
	std::vector<bool> has_normals;
	std::vector<unsigned int> material_index;
	std::vector<std::string> names;

	unsigned int cursize = 0;

	while(getline(in, line)){
		if(line.substr(0, 2) == "v "){
			has_vertices[cursize - 1] = 1;
			glm::vec3 vertex(0);
			sscanf(line.c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			tmp_vertices.push_back(vertex);
			//std::cerr << "v " << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
		}
		else if(line.substr(0, 2) == "vt"){
			has_texcoords[cursize - 1] = 1;
			glm::vec3 texcoord(0);
			sscanf(line.c_str(), "vt %f %f", &texcoord[0], &texcoord[1]);
			//std::cerr << "vt " << texcoord[0] << " " << texcoord[1] << std::endl;
			tmp_texcoords.push_back(texcoord);
		}
		else if(line.substr(0, 2) == "vn"){
			has_normals[cursize - 1] = 1;
			glm::vec3 normal(0);
			sscanf(line.c_str(), "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
			//std::cerr << "vn " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
			tmp_normals.push_back(normal);
		}
		else if(line.substr(0, 2) == "f "){
			//std::cerr << "face" << std::endl;
			unsigned int vert_index[3], norm_index[3], texcoord_index[3];
			if(has_normals[cursize - 1] == 1 && has_texcoords[cursize - 1] == 1){
				sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&vert_index[0], &texcoord_index[0], &norm_index[0],
						&vert_index[1], &texcoord_index[1], &norm_index[1],
						&vert_index[2], &texcoord_index[2], &norm_index[2]);
				indices_vertex[cursize - 1].push_back(vert_index[0]);
				indices_vertex[cursize - 1].push_back(vert_index[1]);
				indices_vertex[cursize - 1].push_back(vert_index[2]);
				indices_normals[cursize - 1].push_back(norm_index[0]);
				indices_normals[cursize - 1].push_back(norm_index[1]);
				indices_normals[cursize - 1].push_back(norm_index[2]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[0]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[1]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[2]);
			}
			else if(has_normals[cursize - 1] == 1 && has_texcoords[cursize - 1] == 0){
				sscanf(line.c_str(), "f %d//%d %d//%d %d//%d",
						&vert_index[0], &norm_index[0],
						&vert_index[1], &norm_index[1],
						&vert_index[2], &norm_index[2]);
				indices_vertex[cursize - 1].push_back(vert_index[0]);
				indices_vertex[cursize - 1].push_back(vert_index[1]);
				indices_vertex[cursize - 1].push_back(vert_index[2]);
				indices_normals[cursize - 1].push_back(norm_index[0]);
				indices_normals[cursize - 1].push_back(norm_index[1]);
				indices_normals[cursize - 1].push_back(norm_index[2]);
			}
			else if(has_normals[cursize - 1] == 0 && has_texcoords[cursize - 1] == 1){
				sscanf(line.c_str(), "f %d/%d %d/%d %d/%d",
						&vert_index[0], &texcoord_index[0],
						&vert_index[1], &texcoord_index[1],
						&vert_index[2], &texcoord_index[2]);
				indices_vertex[cursize - 1].push_back(vert_index[0]);
				indices_vertex[cursize - 1].push_back(vert_index[1]);
				indices_vertex[cursize - 1].push_back(vert_index[2]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[0]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[1]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[2]);
			}
		}
		else if(line.substr(0, 2) == "o "){
			cursize++;
			indices_vertex.resize(cursize);
			indices_texcoords.resize(cursize);
			indices_normals.resize(cursize);
			has_vertices.resize(cursize);
			has_texcoords.resize(cursize);
			has_normals.resize(cursize);
			material_index.resize(cursize);
			names.resize(cursize);
			has_vertices[cursize - 1] = 0;
			has_texcoords[cursize - 1] = 0;
			has_normals[cursize - 1] = 0;
			material_index[cursize - 1] = 0;
			names[cursize - 1] = line.substr(2);

			/*if(cursize > 1) std::cerr << indices_vertex[cursize - 2].size() << std::endl << std::endl;
			else std::cerr << "0" << std::endl << std::endl;
			std::cerr << "new 1" << std::endl;
			std::cerr << line.substr(2) << std::endl;*/
		}
		else if(line.substr(0, 6) == "usemtl"){
			if(indices_vertex[cursize - 1].size() > 0){
				cursize++;
				indices_vertex.resize(cursize);
				indices_texcoords.resize(cursize);
				indices_normals.resize(cursize);
				has_vertices.resize(cursize);
				has_texcoords.resize(cursize);
				has_normals.resize(cursize);
				material_index.resize(cursize);
				names.resize(cursize);
				has_vertices[cursize - 1] = has_vertices[cursize - 2];
				has_texcoords[cursize - 1] = has_texcoords[cursize - 2];
				has_normals[cursize - 1] = has_normals[cursize - 2];
				material_index[cursize - 1] = 0;	//TODO
				names[cursize - 1] = names[cursize - 2];

				/*std::cerr << indices_vertex[cursize - 2].size() << std::endl << std::endl;
				std::cerr << "new 2.1 " << std::endl;
				std::cerr << line.substr(7) << std::endl;*/
			}
			else{
				material_index[cursize - 1] = 0;	//TODO

				/*std::cerr << indices_vertex[cursize - 1].size() << std::endl << std::endl;
				std::cerr << "new 2.2 " << std::endl;
				std::cerr << line.substr(7) << std::endl;*/
			}
		}
		else if(line.substr(0, 2) == "# "){
			//ignore
		}
		else{
			//implement later
		}
		//std::cerr << (char)in.peek() << std::endl;
	}

	for(unsigned int i = 0; i < indices_vertex.size(); i++){
		obj_mesh *m = new obj_mesh;
		if(has_vertices[i] == 1){
			m->has_vertices = 1;
			for(unsigned int j = 0; j < indices_vertex[i].size(); j++){
				indices_vertex[i][j] -= 1;
				m->vertices.push_back(tmp_vertices[indices_vertex[i][j]]);
			}
		}
		if(has_texcoords[i] == 1){
			m->has_texcoords = 1;
			for(unsigned int j = 0; j < indices_texcoords[i].size(); j++){
				indices_texcoords[i][j] -= 1;
				m->texcoords.push_back(tmp_texcoords[indices_texcoords[i][j]]);
			}
		}
		if(has_normals[i] == 1){
			m->has_normals = 1;
			for(unsigned int j = 0; j < indices_normals[i].size(); j++){
				indices_normals[i][j] -= 1;
				m->normals.push_back(tmp_normals[indices_normals[i][j]]);
			}
		}
		meshes.push_back(m);

		/*std::cerr << "vertex_size: " << m->vertices.size() << std::endl;
		std::cerr << "texcoords_size: " << m->texcoords.size() << std::endl;
		std::cerr << "normals_size: " << m->normals.size() << std::endl << std::endl;*/
	}

	/*for(unsigned int i = 0; i < vertices.size(); i++)
		std::cerr << "vertices: " << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << std::endl;
	for(unsigned int i = 0; i < normals.size(); i++)
		std::cerr << "normals: " << normals[i][0] << " " << normals[i][1] << " " << normals[i][2] << std::endl;
	for(unsigned int i = 0; i < texcoords.size(); i++)
		std::cerr << "texcoords: " << i + 1 << " : " << texcoords[i][0] << " " << texcoords[i][1] << std::endl;*/

	/*std::cerr << "bla" << std::endl;
	for(unsigned int i = 0; i < meshes.size(); i++){
		for(unsigned int j = 0; j < meshes[i]->vertices.size(); j++){
			std::cerr << "mesh: " << j << " vertices: " << meshes[i]->vertices[j][0] << " " << meshes[i]->vertices[j][1] << " " << meshes[i]->vertices[j][2] << std::endl;
		}
	}*/

	std::cerr << "loaded successfully: " << file << std::endl;
	return true;
}

bool model::load_to_buffers(){
	//std::cerr << meshes.size() << std::endl;
	for(unsigned int i = 0; i < meshes.size(); i++){
		glGenVertexArrays(1, &meshes[i]->vao);
		glBindVertexArray(meshes[i]->vao);

		if(meshes[i]->has_vertices){
			glGenBuffers(1, &meshes[i]->vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, meshes[i]->vertices.size() * sizeof(glm::vec3), &meshes[i]->vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);
		}

		if(meshes[i]->has_texcoords){
			glGenBuffers(1, &meshes[i]->uvbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->uvbuffer);
			glBufferData(GL_ARRAY_BUFFER, meshes[i]->texcoords.size() * sizeof(glm::vec3), &meshes[i]->texcoords[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);
		}
	}
	std::cerr << "loaded to buffers successfully: " << name << std::endl;
	return true;
}

//###############################################################  transformation matrix
glm::mat4 model::get_trans_mat(){
	return trans;
}

void model::set_trans_mat(glm::mat4 mat){
	trans = mat;
	return;
}

//###############################################################  move
glm::mat4 model::move(glm::vec3 mov){
	trans *= glm::translate(glm::mat4(1.f), mov);
	return trans;
}

glm::mat4 model::rotate(float degrees, glm::vec3 axis){
	trans *= glm::rotate(glm::mat4(1.f), degrees, axis);
	return trans;
}

glm::mat4 model::scale(glm::vec3 scale){
	trans *= glm::scale(glm::mat4(1.f), scale);
	return trans;
}

//###############################################################  render
void model::render(){
	glUniformMatrix4fv(uniform_trans, 1, GL_FALSE, glm::value_ptr(trans));
	for(unsigned int i = 0; i < meshes.size(); i++){
		if(meshes[i]->has_vertices){
			glBindVertexArray(meshes[i]->vao);
			glUniform4fv(uniform_color, 1, glm::value_ptr(glm::vec4(0.5f, 0.5f, 0.5f, 1.f)));
			glDrawArrays(GL_TRIANGLES, 0, meshes[i]->vertices.size() * sizeof(glm::vec3));
		}
	}
	return;
}


}
















