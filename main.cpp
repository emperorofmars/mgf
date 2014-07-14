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

	mvm::fmat44 proj = mvm::perspective(90.0f, 1.0f, 0.5f, 1000.f);

	mol::mesh<float> model("res/models/car_1.obj");

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,  model.get_size(), model.get_vertices(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER,  sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

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
			else if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
				case SDLK_q:
					quit = true;
					break;
				}
			}
		}
		float currentTime = SDL_GetTicks() / 1000.f;
//###############################################  Rendering
		g.current_window(0);
		p.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);

		float f = currentTime * 0.2f;
		mvm::fmat44 trans;
		trans.identity_mat();
		trans = trans *
			mvm::translate(0.f, -5.f, -25.f) *
			//mvm::translate(sinf(2.1f * f) * 0.5f, cosf(1.7f * f) * 0.5f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
			mvm::scale(2.f, 2.f, 2.f) *
			//mvm::rot(0.f, currentTime * (float)M_PI / 2.f, 1.f) *
			//mvm::rot(currentTime * (float)M_PI * 0.9f, currentTime * 0.5f, currentTime);
			mvm::rot(-0.3f, currentTime, 0.0f);

        glUniformMatrix4fv(mv_location, 1, GL_FALSE, trans.data);
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj.data);
        //glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj_matrix);

		glDrawArrays(GL_TRIANGLES, 0, model.get_num());

		g.swap_window(0);
	}
//###############################################  Gameloop end
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &vao);

	return 0;
}











