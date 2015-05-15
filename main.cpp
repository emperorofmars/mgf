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
	std::shared_ptr<mgf::IInput> input(new mgf::InputTopDown);

	std::shared_ptr<mgf::ShaderProgram> p(new mgf::ShaderProgram);
	p->addShader("res/shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	p->addShader("res/shader/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	p->createProgram();

	std::shared_ptr<mgf::ICamera> cam(new mgf::CameraTopDown(45, w->getAspectRatio(), 1000.f, 0.1f));
	cam->setPos(glm::vec3(0.f, 50.f, 15.f));
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
	std::shared_ptr<mgf::MeshNode> mnode(new mgf::MeshNode("MousePos"));
	mnode->addMesh(mgf::createCube());
	mnode->scale(glm::vec3(0.5f, 2.f, 0.5f));
	actualScene->add(mnode);
	actualScene->print();

//###############################################  transform objects
	actualScene->getChild("Suzanne")->scale(glm::vec3(1.f, 1.5f, 2.f));
	actualScene->getChild("Suzanne")->translate(glm::vec3(5.f, -5.f, 2.f));
	actualScene->getChild("Cube")->translate(glm::vec3(-6.f, 1.f, 4.f));
	actualScene->getChild("Cube")->scale(glm::vec3(1.f, 1.f, 4.f));

	std::shared_ptr<mgf::Material> newmat(new mgf::Material);
	newmat->mDiffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	actualScene->getChild("Cube")->setMaterial(newmat);
	//actualScene->getChild("Cube")->resetMaterial();

	std::shared_ptr<mgf::Moveable> ui(new mgf::Moveable("ui"));
	//ui->translate(glm::vec2(5.f, 5.f));
	//ui->scale(glm::vec2(2.f, 2.f));

//###############################################  Gameloop
	float current = 0, last = 0, frametime = 0;
	bool quit = false;
	while(quit != true){
//###############################################  Update
		input->update();
		quit = input->getQuit();
		cam->update(input->getPosition(), input->getMouseRelative());

		actualScene->getChild("Cube")->rotate(0.02f, glm::vec3(0.f,1.f, 0.f)); //rotate the cube
//###############################################  Mouse
		glm::vec3 mray = mgf::calculateMouseRay(cam->getP(), cam->getV(), input->getMouseAbsolute(), glm::vec2(1000, 800));
		glm::vec3 mpoint = mgf::colLinePlane(cam->getPos(), mray, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

		actualScene->getChild("MousePos")->setTranslation(mpoint);
//###############################################  Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

		actualScene->render(renderer); //rendering on gpu happens here
		ui->render(renderer);

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











