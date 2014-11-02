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

	mgf::scene scene("res/models/cube/cube.obj", true);
	mgf::scene scene1("res/models/scene.obj", true);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	p.use();

	scene.translate("Cube", glm::vec3(0.f, 10.f, -1.f));
	scene.scale("Cube", glm::vec3(2.f, 2.f, 2.f));
	scene1.translate("3DSMesh_0", glm::vec3(0.f, 1.f, 0.f));
	scene1.translate("scene.p", glm::vec3(0.f, -2.f, 0.f));
	scene1.translate("<3DSDummyRoot>", glm::vec3(0.f, -2.f, 0.f));
	scene1.translate("3DSMesh_0", glm::vec3(0.f, 1.f, 0.f));
//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Update
		float currentTime = SDL_GetTicks() / 1000.f;
		input.update();
		quit = input.get_quit();
		cam.update(input.get_pos(), input.get_rot());

		scene.rotate("Cube", 0.03f, glm::vec3(0.f, 1.f, 0.f));
		scene1.rotate("3DSMesh_0", 0.01f, glm::vec3(0.f, 1.f, 0.f));
		scene1.rotate("Cube", 0.01f, glm::vec3(0.f, 1.f, 0.f));
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

		scene.render(cam, p);
		scene1.render(cam, p);

		g.swap_window(0);
	}
//###############################################  Gameloop end
	SDL_SetRelativeMouseMode(SDL_FALSE);

	std::cerr << std::endl << "closing main!" << std::endl;
	return 0;
}











