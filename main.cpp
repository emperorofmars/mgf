/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"
//###############################################  Main
int main(int argc, char *argv[]){
	mgf::window w(1000, 800, 0, 0);
	if(!mgf::mgf_init(OPENGL_3_3)) return 1;

	mgf::window w2(600, 400, 0, 1);

	mgf::input input;

	mgf::shader_program p;
	p.add_shader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	//p.add_shader("res/shader/tesselationcontrol_shader.glsl", GL_TESS_CONTROL_SHADER);
	//p.add_shader("res/shader/tesselationevaluation_shader.glsl", GL_TESS_EVALUATION_SHADER);
	//p.add_shader("res/shader/geometry_shader.glsl", GL_GEOMETRY_SHADER);
	p.add_shader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p.create_program();

	mgf::camera cam(90 * M_PI / 180, w.get_aspect_ratio(), 0.1f, 1000.f, 2, 0.6f, 0.4f);

	int loaderflags = 0;
	if(INTEL_GPU == 1) loaderflags |= LOAD_NO_INDICES;	//weil intel gpus zu blöd für inexed draws sind
	mgf::scene *scene1 = mgf::load("res/models/cube/cube.obj", loaderflags);
	mgf::load_into_scene(scene1, "res/models/scene/scene.obj", loaderflags);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	//mgf::render_info.update_camera(&cam);
	//mgf::render_info.update_program(&p);

	w.use();
	p.use();
	cam.use();

	//scene->repo_translate("supercube", glm::vec3(0.f, 10.f, -1.f));
	//scene->repo_scale("supercube", glm::vec3(2.f, 2.f, 2.f));
	scene1->repo_translate("scene.obj", glm::vec3(0.f, -5.f, 0.f));

	std::cerr << "TREE: " << std::endl;
	scene1->_root_repository[0]->print();

//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Update
		w.use();
		float currentTime = SDL_GetTicks() / 1000.f;
		input.update();
		quit = input.get_quit();
		cam.update(input.get_pos(), input.get_rot());

		scene1->repo_rotate("supercube", 0.01f, glm::vec3(0.f, 1.f, 0.f));
		scene1->repo_rotate("Suzanne", -0.01f, glm::vec3(0.f, 1.f, 0.f));
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		scene1->render_repository();
		//scene1->_root_repository[0]->find_node("supercube")->render();
		//scene1->_root_repository[0]->find_node("Cube")->render();

		w.swap();
		w2.use();
		scene1->_root_repository[0]->find_node("supercube")->render();
		w2.swap();
	}
//###############################################  Gameloop end
	SDL_SetRelativeMouseMode(SDL_FALSE);

	//delete scene;
	delete scene1;

	std::cerr << std::endl << "closing main!" << std::endl;
	mgf::mgf_close();
	return 0;
}











