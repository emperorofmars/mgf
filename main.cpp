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

	mgf::scene *scene = mgf::load("res/models/cube/cube.obj", 0);
	mgf::scene *scene1 = mgf::load("res/models/scene/scene.obj", 0);
	//mgf::loader scene1("res/models/car/DeLorean_Final.obj", true);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	mgf::render_info.update_camera(&cam);
	mgf::render_info.update_program(&p);

	scene->repo_translate("Cube", glm::vec3(0.f, 10.f, -1.f));
	scene->repo_scale("Cube", glm::vec3(2.f, 2.f, 2.f));
	scene1->repo_translate("scene.obj", glm::vec3(0.f, -2.f, 0.f));
//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Update
		float currentTime = SDL_GetTicks() / 1000.f;
		input.update();
		quit = input.get_quit();
		cam.update(input.get_pos(), input.get_rot());

		/*glm::mat4 trans = scene1._root_node->_trans;
		for(unsigned int i = 0; i < 4; i++){
			for(unsigned int j = 0; j < 4; j++) std::cerr << trans[i][j] << " ";
			std::cerr << std::endl;
		}std::cerr << std::endl;*/

		scene->repo_rotate("Cube", 0.03f, glm::vec3(0.f, 1.f, 0.f));
		scene1->repo_rotate("Cube", 0.01f, glm::vec3(0.f, 1.f, 0.f));
		scene1->repo_rotate("Suzanne", -0.01f, glm::vec3(0.f, 1.f, 0.f));
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		scene->render_repository();
		scene1->render_repository();

		g.swap_window(0);
	}
//###############################################  Gameloop end
	SDL_SetRelativeMouseMode(SDL_FALSE);

	delete scene;
	//delete scene1;

	std::cerr << std::endl << "closing main!" << std::endl;
	return 0;
}











