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

	std::shared_ptr<mgf::IInput> input(new mgf::InputTopDown);

	std::shared_ptr<mgf::ShaderProgram> p(new mgf::ShaderProgram);
	p->addShader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	p->addShader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p->createProgram();

	std::shared_ptr<mgf::ICamera> cam(new mgf::CameraTopDown(45, w->getAspectRatio(), 1000.f, 0.1f));
	cam->setPos(glm::vec3(0.f, 50.f, 15.f));

	std::shared_ptr<mgf::Renderer> renderer(new mgf::Renderer(w, cam, p));

//###############################################  Load 3d files
	mgf::Loader l(false, false);
	std::shared_ptr<mgf::Node> field(new mgf::Node("field"));
	field->add(l.load("res/models/Assets/Assets.obj"));

//###############################################  transform objects
	std::shared_ptr<mgf::Node> actualField = field->getChild("Assets.obj");;

	actualField->getChild("Tree")->translate(glm::vec3(15.f, 0.f, -4.f));
	actualField->getChild("Tree")->add(actualField->getChild("defaultobject")->clone());
	actualField->remove("defaultobject");
	actualField->getChild("Tower01")->translate(glm::vec3(-10.f, 0.f, -4.f));
	actualField->getChild("Tower02")->translate(glm::vec3(-15.f, 0.f, -4.f));
	actualField->getChild("Infantry01")->translate(glm::vec3(-15.f, 0.f, -8.f));
	actualField->getChild("Infantry02")->translate(glm::vec3(-10.f, 0.f, -8.f));
	actualField->getChild("Cavalry01")->translate(glm::vec3(-5.f, 0.f, -8.f));
	actualField->getChild("Cavalry02")->translate(glm::vec3(0.f, 0.f, -8.f));
	actualField->getChild("Artillery01")->translate(glm::vec3(5.f, 0.f, -8.f));
	actualField->getChild("Artillery02")->translate(glm::vec3(10.f, 0.f, -8.f));

	//std::shared_ptr<mgf::Material> newmat(new mgf::Material);
	//newmat->mDiffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	//actualField->getChild("Infantry01")->setMaterial(newmat);
	//actualField->getChild("Infantry01")->resetMaterial();

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
	overlay->add(lab);

//###############################################  create lights
	std::shared_ptr<mgf::Light> light(new mgf::Light());
	light->mColor = glm::vec3(0.8f, 1.f, 0.6f);
	light->mStrengthDiffuse = 1;
	light->mPosition = glm::vec3(5.f, 15.f, 15.f);
	renderer->addLight(light, glm::mat4(1));
	light->mType = 2;
	light->mColor = glm::vec3(0.4f, 0.4f, 0.4f);
	light->mDirection = glm::vec3(0.f, -10.f, -5.f);
	renderer->addLight(light, glm::mat4(1));
//###############################################  Gameloop
	float current = 0, last = 0, frametime = 0;
	bool quit = false;
	while(quit != true){
//###############################################  Update
		input->update();
		quit = input->getQuit();
		cam->update(input->getPosition(), input->getMouseRelative());

		lab->setPos(glm::vec2(input->getMouseAbsoluteNDC(w->getResolution())[0], input->getMouseAbsoluteNDC(w->getResolution())[1] / w->getAspectRatio()));

		std::string col = "nope";
		std::shared_ptr<mgf::IOverlayElement> elm = overlay->getMouseOverNDC(input->getMouseAbsoluteNDC(w->getResolution()), w->getAspectRatio());
		if(elm) col = elm->getName();
		//std::cerr << "BUTTONCOL: " << col << std::endl;

		//actualScene->getChild("Cube")->rotate(0.02f, glm::vec3(0.f,1.f, 0.f)); //rotate the cube
//###############################################  Mouse
		glm::vec3 mray = mgf::calculateMouseRay(cam->getP(), cam->getV(), input->getMouseAbsolute(), glm::vec2(1000, 800));
		glm::vec3 mpoint = mgf::colLinePlane(cam->getPos(), mray, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

		//actualScene->getChild("MousePos")->setTranslation(mpoint);
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		field->render(renderer); //rendering on gpu happens here
		overlay->render(renderer);

		w->swap(); //display the rendered image on screen
//###############################################  Calculate fps
		current = SDL_GetTicks();
		frametime = current - last;
		last = current;
		//std::cerr << "FPS: " << 1000.f / frametime << std::endl;	//show fps

		GLenum error;
		while((error = glGetError()) != GL_NO_ERROR){
			std::cerr << "ERROR: " << error << std::endl;
		}
	}
//###############################################  Gameloop end

	std::cerr << std::endl << "closing main!" << std::endl;
	return 0;
}











