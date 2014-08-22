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

//###############################################################  parse object
bool obj_mesh::parse_object(std::ifstream &in, int *offset){
	std::string line;
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec3> tmp_texcoords;
	std::vector<glm::vec3> tmp_normals;
	std::vector<unsigned int> indices_vertex;
	std::vector<unsigned int> indices_texcoords;
	std::vector<unsigned int> indices_normals;
	vertices.clear();
	texcoords.clear();
	normals.clear();

	getline(in, line);
	name = line.substr(2);
	//std::cerr << name << std::endl;

	while(getline(in, line)){
		//std::cerr << "line: " << in.tellg() << std::endl;
		if(line.substr(0, 2) == "v "){
			has_vertices = 1;
			glm::vec3 vertex(0);
			sscanf(line.c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			tmp_vertices.push_back(vertex);
			//std::cerr << "v " << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
		}
		else if(line.substr(0, 2) == "vt"){
			has_texcoords = 1;
			glm::vec3 texcoord(0);
			sscanf(line.c_str(), "vt %f %f", &texcoord[0], &texcoord[1]);
			//std::cerr << "vt " << texcoord[0] << " " << texcoord[1] << std::endl;
			tmp_texcoords.push_back(texcoord);
		}
		else if(line.substr(0, 2) == "vn"){
			has_normals = 1;
			glm::vec3 normal(0);
			sscanf(line.c_str(), "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
			//std::cerr << "vn " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
			tmp_normals.push_back(normal);
		}
		else if(line.substr(0, 2) == "f "){
			std::istringstream s(line.substr(2));
			unsigned int vert_index[3], norm_index[3], texcoord_index[3];
			if(has_normals == 1 && has_texcoords == 1){
				sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&vert_index[0], &texcoord_index[0], &norm_index[0],
						&vert_index[1], &texcoord_index[1], &norm_index[1],
						&vert_index[2], &texcoord_index[2], &norm_index[2]);
				indices_vertex.push_back(vert_index[0]);
				indices_vertex.push_back(vert_index[1]);
				indices_vertex.push_back(vert_index[2]);
				indices_normals.push_back(norm_index[0]);
				indices_normals.push_back(norm_index[1]);
				indices_normals.push_back(norm_index[2]);
				indices_texcoords.push_back(texcoord_index[0]);
				indices_texcoords.push_back(texcoord_index[1]);
				indices_texcoords.push_back(texcoord_index[2]);
			}
			else if(has_normals == 1 && has_texcoords == 0){
				sscanf(line.c_str(), "f %d//%d %d//%d %d//%d",
						&vert_index[0], &norm_index[0],
						&vert_index[1], &norm_index[1],
						&vert_index[2], &norm_index[2]);
				indices_vertex.push_back(vert_index[0]);
				indices_vertex.push_back(vert_index[1]);
				indices_vertex.push_back(vert_index[2]);
				indices_normals.push_back(norm_index[0]);
				indices_normals.push_back(norm_index[1]);
				indices_normals.push_back(norm_index[2]);
			}
			else if(has_normals == 0 && has_texcoords == 1){
				sscanf(line.c_str(), "f %d/%d %d/%d %d/%d",
						&vert_index[0], &texcoord_index[0],
						&vert_index[1], &texcoord_index[1],
						&vert_index[2], &texcoord_index[2]);
				indices_vertex.push_back(vert_index[0]);
				indices_vertex.push_back(vert_index[1]);
				indices_vertex.push_back(vert_index[2]);
				indices_texcoords.push_back(texcoord_index[0]);
				indices_texcoords.push_back(texcoord_index[1]);
				indices_texcoords.push_back(texcoord_index[2]);
			}
		}
		if(line.substr(0, 2) == "o "){
			break;
		}
		else if(line.substr(0, 2) == "# "){
			//ignore
		}
		else{
			//implement later
		}

		if(in.peek() == 'o'){
			//std::cerr << (char)in.peek() << std::endl;
			break;
		}
	}

	for(unsigned int i = 0; i < indices_vertex.size(); i++){
		indices_vertex[i] -= 1;
		//std::cerr << "offset_vertex: " << indices_vertex[i] - offset[2] + 1 << std::endl;
		vertices.push_back(tmp_vertices[indices_vertex[i] - offset[0]]);
	}
	offset[0] += tmp_vertices.size();
	if(has_texcoords){
		for(unsigned int i = 0; i < indices_texcoords.size(); i++){
			indices_texcoords[i] -= 1;
			texcoords.push_back(tmp_texcoords[indices_texcoords[i] - offset[1]]);
		}
		offset[1] += tmp_texcoords.size();
	}
	if(has_normals){
		for(unsigned int i = 0; i < indices_normals.size(); i++){
			indices_normals[i] -= 1;
			//std::cerr << "offset_normal: " << indices_normals[i] - offset[2] + 1 << std::endl;
			normals.push_back(tmp_normals[indices_normals[i] - offset[2]]);
		}
		offset[2] += tmp_normals.size();
	}

	/*for(unsigned int i = 0; i < vertices.size(); i++)
		std::cerr << "vertices: " << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << std::endl;
	for(unsigned int i = 0; i < normals.size(); i++)
		std::cerr << "normals: " << normals[i][0] << " " << normals[i][1] << " " << normals[i][2] << std::endl;
	for(unsigned int i = 0; i < texcoords.size(); i++)
		std::cerr << "texcoords: " << i + 1 << " : " << texcoords[i][0] << " " << texcoords[i][1] << std::endl;*/

	/*std::cerr << "vertex_size: " << vertices.size() << std::endl;
	std::cerr << "texcoords_size: " << texcoords.size() << std::endl;
	std::cerr << "normals_size: " << normals.size() << std::endl << std::endl;*/

	return true;
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
	int offset[3];
	offset[0] = 0; offset[1] = 0; offset[2] = 0;

	while(1){
		if(in.peek() == 'o'){
			m->parse_object(in, offset);
			meshes.push_back(m);
			m = new obj_mesh;
		}
		else if(in.eof() == false){
			//std::cerr << (char)in.peek() << std::endl;
			getline(in, line);
		}
		else break;
	}

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

//###############################################################  render
void model::render(){
	glUniformMatrix4fv(uniform_trans, 1, GL_FALSE, glm::value_ptr(trans));
	for(unsigned int i = 0; i < meshes.size(); i++){
		glBindVertexArray(meshes[i]->vao);
		glUniform4fv(uniform_color, 1, glm::value_ptr(glm::vec4(0.5f, 0.5f, 0.5f, 1.f)));
		glDrawArrays(GL_TRIANGLES, 0, meshes[i]->vertices.size() * sizeof(glm::vec3));
	}
	return;
}


}
















