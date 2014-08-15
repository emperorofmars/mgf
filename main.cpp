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

	mgf::camera cam;

	GLuint mv_location = glGetUniformLocation(p.get_program(), "mv_matrix");
	GLuint proj_location = glGetUniformLocation(p.get_program(), "proj_matrix");

	mgf::object model("res/models/car_2.obj");
	//car_1
	//suzanne

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);// Enable depth test
	glDepthFunc(GL_LESS);// Accept fragment if it closer to the camera than the former one

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPointSize(5);

	//SDL_SetRelativeMouseMode(SDL_TRUE);

	glm::vec3 pos, rot;
//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Input Handling
		//rot = glm::vec3(0);
		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT) quit = true;
			else{
				if(event.type == SDL_KEYDOWN){
					switch(event.key.keysym.sym){
					case SDLK_q:
						quit = true;
						break;
					}
				}
			}
		}

		float currentTime = SDL_GetTicks() / 1000.f;

		glm::mat4 vp = cam.update(pos, rot);

		SDL_WarpMouseInWindow(g.mWindows[0].window, 400, 400);

//###############################################  Rendering
		g.current_window(0);
		p.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);

		glm::mat4 trans(1.0f), lookat;
		trans = trans *
			glm::translate(glm::mat4(1.f), glm::vec3(0.f, -5.f, -30.f)) *
			glm::scale(glm::mat4(1.f), glm::vec3(2.f, 2.f, 2.f));

		glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(vp));

		model.render();

		g.swap_window(0);
	}
//###############################################  Gameloop end

	//SDL_SetRelativeMouseMode(SDL_FALSE);

	return 0;
}











