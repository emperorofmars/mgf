/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"
//###############################################  Main
int main(int argc, char *argv[]){
	mgf::init g(1000, 800, 0, 0, 0);
	if(!g.init_all()) return 1;

	mgf::input input;

	mgf::shader_program p;
	p.add_shader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	//p.add_shader("res/shader/tesselationcontrol_shader.glsl", GL_TESS_CONTROL_SHADER);
	//p.add_shader("res/shader/tesselationevaluation_shader.glsl", GL_TESS_EVALUATION_SHADER);
	//p.add_shader("res/shader/geometry_shader.glsl", GL_GEOMETRY_SHADER);
	p.add_shader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p.create_program();

	mgf::camera cam(90 * M_PI / 180, g.get_aspect_ratio(), 0.1f, 1000.f, 2, 0.6f, 0.4f);

	std::vector<mgf::model *> models;
	mgf::model model_cube("res/models/cube/cube.obj");
	mgf::model model_car("res/models/car/DeLorean_Final.obj");
	models.push_back(&model_cube);
	models.push_back(&model_car);

	model_car.scale(glm::vec3(2.f, 2.f, 2.f));
	model_car.move(glm::vec3(0.f, -10.f, -5.f));
	model_cube.move(glm::vec3(0.f, 0.f, -20.f));
	model_cube.scale(glm::vec3(4.f, 4.f, 4.f));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	p.use();

//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Update
		float currentTime = SDL_GetTicks() / 1000.f;
		input.update();
		quit = input.get_quit();
		cam.update(input.get_pos(), input.get_rot());

//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

		model_cube.rotate(0.02, glm::vec3(0.f, 1.f, 0.f));
		mgf::render(models, cam, p.get_program());

		g.swap_window(0);
	}
//###############################################  Gameloop end
	SDL_SetRelativeMouseMode(SDL_FALSE);

	std::cerr << std::endl << "closing main!" << std::endl;
	return 0;
}











