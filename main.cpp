/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"
//###############################################  Main
int main(int argc, char *argv[]){
	LOG_INIT("log_mgf.txt", true);

	std::shared_ptr<mgf::Window> w(new mgf::Window("MGF Test", 1000, 800, 0, 0));
	std::shared_ptr<mgf::Input> input(new mgf::Input);

	std::shared_ptr<mgf::ShaderProgram> p(new mgf::ShaderProgram);
	p->addShader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	p->addShader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p->createProgram();

	std::shared_ptr<mgf::Camera> cam(new mgf::Camera(90, w->getAspectRatio(), 0.1f, 1000.f, CAMERA_FPS_EULER, 0.6f, 0.4f));

	std::shared_ptr<mgf::Renderer> renderer(new mgf::Renderer(w, cam, p));

	mgf::Loader l;
	std::shared_ptr<mgf::Node> root = l.load("res/models/cube/cube.obj"); //"res/models/scene/scene.obj"
	if(!root){
		LOG_F_ERROR(MGF_LOG_FILE, "Loading failed, exiting main!");
		return 0;
	}
	root->add(l.load("res/models/scene/scene.obj"));
/*
	std::shared_ptr<mgf::Node> node(new mgf::Node("root"));
	node->add(std::shared_ptr<mgf::Node>(new mgf::MeshNode("child1")));
	node->print();
	node->translate(glm::vec3(0.f, 1.f, 0.f));
	std::shared_ptr<mgf::MeshNode> tmp = std::dynamic_pointer_cast<mgf::MeshNode>(node->getChild("child1"));
	tmp->translate(glm::vec3(5.f, 0.f, 0.f));
	tmp->rotate(20.f, glm::vec3(0.3f, 1.f, 0.1f));
	tmp->scale(glm::vec3(4.f, 1.f, 1.f));
	tmp->addMesh(mgf::createCube());

	std::cout << "TRANSLATION: " << mgf::vec3_toStr(tmp->getGlobalTranslation()) << std::endl;
	std::cout << "ROTATION: " << std::endl << mgf::mat4_toStr(tmp->getGlobalRotation()) << std::endl;
	std::cout << "TRS: " << std::endl << mgf::mat4_toStr(tmp->getGlobalTRS()) << std::endl;
*/

	SDL_SetRelativeMouseMode(SDL_TRUE);

	w->use();
	p->use();

//###############################################  Gameloop
	float current = 0, last = 0, frametime = 0;
	bool quit = false;
	while(quit != true){
//###############################################  Update
		input->update();
		quit = input->getQuit();
		cam->update(input->getPos(), input->getRot());
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		SDL_Delay(16);

		root->render(renderer);

		w->swap();

		current = SDL_GetTicks();
		frametime = current - last;
		last = current;
		//std::cerr << "FPS: " << 1000.f / frametime << std::endl;	//show fps
	}
//###############################################  Gameloop end

	std::cerr << std::endl << "closing main!" << std::endl;
	return 0;
}











