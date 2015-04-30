/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"
//###############################################  Main
int main(int argc, char *argv[]){
	LOG_INIT("log_mgf.txt", true);

//###############################################  Create necessary objects
	std::shared_ptr<mgf::Window> w(new mgf::Window("MGF Test", 1000, 800, 0, 0));
	std::shared_ptr<mgf::Input> input(new mgf::Input);

	std::shared_ptr<mgf::ShaderProgram> p(new mgf::ShaderProgram);
	p->addShader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	p->addShader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p->createProgram();

	std::shared_ptr<mgf::ICamera> cam(new mgf::CameraTopDown(90, w->getAspectRatio(), 1000.f, 0.1f));
	std::shared_ptr<mgf::Renderer> renderer(new mgf::Renderer(w, cam, p));

//###############################################  Load 3d files
	mgf::Loader l;
	std::shared_ptr<mgf::Node> root(new mgf::Node("root"));
	root->add(l.load("res/models/cube/cube.obj"));
	root->add(l.load("res/models/scene/scene.obj"));
	root->print();

//###############################################  clone objects into szene
	std::shared_ptr<mgf::Node> actualScene(new mgf::Node("scene"));
	actualScene->add(root->getChild("scene.obj")->getChild("Cube")->clone());
	actualScene->add(root->getChild("scene.obj")->getChild("Suzanne")->clone());
	actualScene->print();

//###############################################  transform objects
	actualScene->getChild("Suzanne")->scale(glm::vec3(1.f, 1.5f, 2.f));
	actualScene->getChild("Suzanne")->translate(glm::vec3(5.f, -5.f, 2.f));
	actualScene->getChild("Cube")->translate(glm::vec3(2.f, 1.f, 4.f));
	actualScene->getChild("Cube")->scale(glm::vec3(1.f, 1.f, 4.f));

//###############################################  Gameloop
	float current = 0, last = 0, frametime = 0;
	bool quit = false;
	while(quit != true){
//###############################################  Update
		input->update();
		quit = input->getQuit();
		cam->update(input->getPos(), input->getRot());

		actualScene->getChild("Cube")->rotate(0.02f, glm::vec3(0.f,1.f, 0.f)); //rotate the cube
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		actualScene->render(renderer); //rendering on gpu happens here

		w->swap(); //display the rendered image on screen
//###############################################  Calculate fps
		current = SDL_GetTicks();
		frametime = current - last;
		last = current;
		//std::cerr << "FPS: " << 1000.f / frametime << std::endl;	//show fps
	}
//###############################################  Gameloop end

	std::cerr << std::endl << "closing main!" << std::endl;
	return 0;
}











