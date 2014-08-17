/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"
//###############################################  Main
int main(int argc, char *argv[]){
	SDL_Event event;
	mgf::mgf g(800, 800, 0, 0, 0);
	if(!g.init()) return 1;

	mgf::input input;

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

	mgf::object model("res/models/cube.obj");

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPointSize(5);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	//TEXTURE
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int x, y, n;
	unsigned char *data = stbi_load("res/images/cube.png", &x, &y, &n, 0);
	if(data == NULL) std::cerr << "loading image FAILED" << std::endl;
	else std::cerr << "loading image succesfull" << std::endl;

	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, x, y);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, GL_RGBA, GL_INT, data);
	if(n == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if(n == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
	//TEXTURE END

//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Input Handling
		float currentTime = SDL_GetTicks() / 1000.f;

		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT) quit = true;
			else{
				if(event.type == SDL_KEYDOWN){
					switch(event.key.keysym.sym){
					case SDLK_q:
						quit = true;
						break;
					case SDLK_ESCAPE:
						if(SDL_GetRelativeMouseMode() == SDL_TRUE)
							SDL_SetRelativeMouseMode(SDL_FALSE);
						else
							SDL_SetRelativeMouseMode(SDL_TRUE);
						break;
					}
				}
			}
		}

		input.update();
		glm::mat4 vp = cam.update(input.get_pos(), input.get_rot());

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
	SDL_SetRelativeMouseMode(SDL_FALSE);

	std::cerr << std::endl << "closing main!" << std::endl;
	return 0;
}











