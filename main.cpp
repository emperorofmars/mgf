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
	std::shared_ptr<mgf::Node> field(new mgf::Node("field"));
	field->add(lt.load("res/models/cube/cube.obj"));
	//field->add(l.load("res/models/scene/scene.obj"));

	field->print();

//###############################################  transform objects

	std::shared_ptr<mgf::Node> actualField = field->getChild("scene.obj");
	std::shared_ptr<mgf::Node> cube = field->getChild("cube.obj")->getChild("supercube");
	
	cube->translate(glm::vec3(-3.f, 3.f, 0.f));
	cube->scale(glm::vec3(1.5f, 1.5f, 1.5f));
	
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
	light1->setLight(mgf::SUN_LIGHT, 3, 2, glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1), glm::vec3(-1.f, -2.f, -1.f), 30);
	field->add(light1);

	std::shared_ptr<mgf::Node> light2(new mgf::LightNode("spot"));
	light2->setLight(mgf::SPOT_LIGHT, 4, 0.5, glm::vec3(0.8f, 1.f, 0.6f), glm::vec3(0.f, 5.f, -5.f), glm::vec3(0.f, -1.f, 2.f), 45);
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
		//std::cerr << "BUTTONCOL: " << col << std::endl;

		field->getChild("spot")->rotate(-0.05f, glm::vec3(0.f, 1.f, 0.f));

//###############################################  Mouse
		//glm::vec3 mray = mgf::calculateMouseRay(cam->getP(), cam->getV(), input->getMouseAbsolute(), glm::vec2(1000, 800));
		//glm::vec3 mpoint = mgf::colLinePlane(cam->getPos(), mray, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		//actualScene->getChild("MousePos")->setTranslation(mpoint);

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











