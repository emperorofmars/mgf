/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"
//###############################################  Main
int main(int argc, char *argv[]){
	mgf::window w(1000, 800, 0, 0);

	renderer_enum renderer_flag;
	if(OPENGL_VERSION == 33) renderer_flag = OPENGL_3_3;
	else if(OPENGL_VERSION == 43) renderer_flag = OPENGL_4_3;

	if(!mgf::mgf_init(renderer_flag)) return 1;

	mgf::input input;

	mgf::shader_program p;
	p.add_shader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	//p.add_shader("res/shader/tesselationcontrol_shader.glsl", GL_TESS_CONTROL_SHADER);
	//p.add_shader("res/shader/tesselationevaluation_shader.glsl", GL_TESS_EVALUATION_SHADER);
	//p.add_shader("res/shader/geometry_shader.glsl", GL_GEOMETRY_SHADER);
	p.add_shader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p.create_program();

	mgf::camera cam(90, w.get_aspect_ratio(), 0.1f, 1000.f, 2, 0.6f, 0.4f);

	int loaderflags = 0;
	if(INTEL_GPU == 1) loaderflags |= LOAD_NO_INDICES;	//weil intel gpus zu blöd für inexed draws sind
	mgf::scene *scene = mgf::load("res/models/cube/cube.obj", loaderflags);
	mgf::load_into_scene(scene, "res/models/scene/scene.obj", loaderflags);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	w.use();
	p.use();
	cam.use();

	scene->translate("scene.obj", glm::vec3(0.f, -5.f, 0.f));

	std::cerr << "TREE: " << std::endl;
	scene->_root_instances[0]->print();

//###############################################  Gameloop
	bool quit = false;
	while(quit != true){
//###############################################  Update
		w.use();
		float currentTime = SDL_GetTicks() / 1000.f;
		input.update();
		quit = input.get_quit();
		cam.update(input.get_pos(), input.get_rot());

		scene->rotate("supercube", 0.01f, glm::vec3(0.f, 1.f, 0.f));
		scene->rotate("Suzanne", -0.01f, glm::vec3(0.f, 1.f, 0.f));
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		scene->render();
		//scene->_root_instances[0]->find_node("supercube")->render();
		//scene->_root_instances[0]->find_node("Cube")->render();

		w.swap();
	}
//###############################################  Gameloop end
	SDL_SetRelativeMouseMode(SDL_FALSE);

	//delete scene;
	delete scene;

	std::cerr << std::endl << "closing main!" << std::endl;
	mgf::mgf_close();
	return 0;
}











