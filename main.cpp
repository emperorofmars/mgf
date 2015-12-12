/*
 *	Author:		Martin Schwarz
 *	Name:		main.cpp
 *	Project:	opengl_o1
 */

#include "gl_test.h"
//###############################################  Main
int main(int argc, char *argv[]){
	LOG_INIT("log_mgf.txt", true);
	mLog::Instance()->setLogLevel(2, 6);

//###############################################  Create necessary objects
	std::shared_ptr<mgf::Window> w(new mgf::Window("MGF Test", 1000, 800, 0, 0));

	std::shared_ptr<mgf::IInput> input(new mgf::InputFPS);

	std::shared_ptr<mgf::ShaderProgram> p(new mgf::ShaderProgram);
	p->addShader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	p->addShader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p->createProgram();

	std::shared_ptr<mgf::ICamera> cam(new mgf::CameraFPSEuler(95, w->getAspectRatio(), 1000.f, 0.1f));
	cam->setPos(glm::vec3(-10.f, 5.f, 0.f));

	std::shared_ptr<mgf::Renderer> renderer(new mgf::Renderer(w, cam, p));

//###############################################  Load 3d files
	mgf::Loader l(false, false);
	mgf::LoaderTransparent lt;
	mgf::LoaderLOD ll;
	std::shared_ptr<mgf::Node> field(new mgf::Node("field"));
	
	auto cubeNode1 = lt.load("res/models/cube/cube.obj");
	auto cubeNode2 = cubeNode1->clone();
	auto cubeNode3 = cubeNode2->clone();
	field->add(cubeNode1);
	field->add(cubeNode2);
	field->add(cubeNode3);
	
	auto lodNode1 = ll.load("lodmesh", "res/models/lod/tree_high.obj", "res/models/lod/tree_medium.obj", "res/models/lod/tree_low.obj", 30, 50);
	auto lodNode2 = lodNode1->clone();
	auto lodNode3 = lodNode2->clone();
	field->add(lodNode1);
	field->add(lodNode2);
	field->add(lodNode3);
	
	field->add(l.load("res/models/scene/scene.obj"));

	field->print();

//###############################################  transform objects
	
	cubeNode1->translate(glm::vec3(-3.f, 4.f, 3.f));
	cubeNode1->scale(glm::vec3(1.8f, 1.8f, 1.8f));
	
	cubeNode2->translate(glm::vec3(+3.f, 5.f, 3.f));
	
	cubeNode3->translate(glm::vec3(0.f, 7.f, 3.f));
	cubeNode3->scale(glm::vec3(6.f, 6.f, 6.f));
	
	lodNode1->translate(glm::vec3(0.f, 0.f, -20.f));
	
	lodNode2->translate(glm::vec3(-10.f, 0.f, -20.f));
	
	lodNode3->translate(glm::vec3(10.f, 0.f, -20.f));
	
//###############################################  create overlay

	std::shared_ptr<mgf::Overlay> overlay(new mgf::Overlay());

	std::shared_ptr<mgf::Button> but(new mgf::Button("but"));
	but->setColor(glm::vec3(1.f, 0.5f, 0.5f));
	but->setFont("res/fonts/main.ttf");
	but->setText("blah");
	but->setBackground("res/images/Button.png");

	std::shared_ptr<mgf::Label> lab(new mgf::Label("mouse"));
	lab->setBackground("res/images/Mouse.png");
	lab->translate(glm::vec2(-10.f, -10.f));

	overlay->add(but);

//###############################################  create lights
	std::shared_ptr<mgf::Node> light1(new mgf::LightNode("sun"));
	light1->setLight(mgf::SUN_LIGHT, 3, 2, glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1), glm::vec3(-1.f, -2.f, -1.f), 30);
	field->add(light1);

	std::shared_ptr<mgf::Node> light2(new mgf::LightNode("spot"));
	light2->setLight(mgf::SPOT_LIGHT, 4, 0.5, glm::vec3(0.8f, 1.f, 0.6f), glm::vec3(-20.f, 5.f, -5.f), glm::vec3(1.f, 0.f, 0.f), 45);
	field->add(light2);

//###############################################  Gameloop
	float current = 0, last = 0, frametime = 0;
	bool quit = false;
	while(quit != true){
//###############################################  Update
		input->update();
		quit = input->getQuit();
		cam->update(input->getPosition() * 0.3f, input->getMouseRelative() * 0.5f);

		lab->setPos(glm::vec2(input->getMouseAbsoluteNDC(w->getResolution())[0], input->getMouseAbsoluteNDC(w->getResolution())[1] / w->getAspectRatio()));

		std::string col = "nope";
		std::shared_ptr<mgf::IOverlayElement> elm = overlay->getMouseOverNDC(input->getMouseAbsoluteNDC(w->getResolution()), w->getAspectRatio());
		if(elm) col = elm->getName();

//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		field->render(renderer);
		
		overlay->render(renderer);

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











