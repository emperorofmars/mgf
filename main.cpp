/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"

//###############################################  Main
int main(int argc, char *argv[]){
	SDL_Event event;
	mgf::mgf g(800, 800, 1);
	if(!g.init()) return 1;

	mgf::shader_program p;
	p.add_shader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	//p.add_shader("res/shader/tesselationcontrol_shader.glsl", GL_TESS_CONTROL_SHADER);
	//p.add_shader("res/shader/tesselationevaluation_shader.glsl", GL_TESS_EVALUATION_SHADER);
	//p.add_shader("res/shader/geometry_shader.glsl", GL_GEOMETRY_SHADER);
	p.add_shader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p.create_program();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint mv_location = glGetUniformLocation(p.get_program(), "mv_matrix");
	GLuint proj_location = glGetUniformLocation(p.get_program(), "proj_matrix");

	glm::mat4 proj = glm::perspective(90.0f, 1.0f, 0.5f, 1000.f);
	glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(-3.0f, 0.0f ,0.0f));

	float dir[3];
	for(int i = 0; i < 3; i++) dir[i] = 0;

	mgf::object model("res/models/car_1.obj");
	//car_1
	//suzanne

	/*GLuint atom_buf;
	glGenBuffers(1, &atom_buf);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atom_buf);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atom_buf);*/

	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);// Enable depth test
	glDepthFunc(GL_LESS);// Accept fragment if it closer to the camera than the former one

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPointSize(5);
//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Input Handling
		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT) quit = true;
			else{
				if(event.type == SDL_KEYDOWN){
					switch(event.key.keysym.sym){
					case SDLK_q:
						quit = true;
						break;
					case SDLK_w:
						dir[2] = 0.3;
						break;
					case SDLK_s:
						dir[2] = -0.3;
						break;
					case SDLK_e:
						dir[1] = 0.3;
						break;
					case SDLK_f:
						dir[1] = 0.3;
						break;
					case SDLK_r:
						dir[1] = -0.3;
						break;
					case SDLK_a:
						dir[0] = 0.3;
						break;
					case SDLK_d:
						dir[0] = -0.3;
						break;
					}
				}
				if(event.type == SDL_KEYUP){
					switch(event.key.keysym.sym){
					case SDLK_w:
						dir[2] = 0;
						break;
					case SDLK_s:
						dir[2] = 0;
						break;
					case SDLK_e:
						dir[1] = 0;
						break;
					case SDLK_f:
						dir[1] = 0;
						break;
					case SDLK_r:
						dir[1] = 0;
						break;
					case SDLK_a:
						dir[0] = 0;
						break;
					case SDLK_d:
						dir[0] = 0;
						break;
					}
				}
			}
		}
		view[3][0] += dir[0];
		view[3][1] += dir[1];
		view[3][2] += dir[2];
		float currentTime = SDL_GetTicks() / 1000.f;

		/*glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atom_buf);
		GLuint *atom_data = (GLuint *)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
		std::cerr << *atom_data << std::endl;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atom_buf);
		atom_data = (GLuint *)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
		*atom_data = 0;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);*/

//###############################################  Rendering
		g.current_window(0);
		p.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);

		glm::mat4 trans(1.0f), lookat;
		trans = trans *
			glm::translate(glm::mat4(1.f), glm::vec3(0.f, -5.f, -30.f)) *
			glm::scale(glm::mat4(1.f), glm::vec3(2.f, 2.f, 2.f)) *
			glm::rotate(glm::mat4(1.f), currentTime * 180.f / (float)M_PI, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.f), 20.f, glm::vec3(1.0f, 0.0f, 0.0f));

		lookat = proj * view;

		glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(lookat));

		model.render();

		g.swap_window(0);
	}
//###############################################  Gameloop end
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &vao);

	return 0;
}











