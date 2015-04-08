/*
**	Author:		Martin Schwarz
**	Name:		mgf_render.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#ifndef MGF_RENDER
#define MGF_RENDER

#include "mgf_include.h"
#include "mgf_node.h"
#include "mgf_shader.h"
#include "mgf_camera.h"

namespace mgf{

class Renderer{
public:
	Renderer();
	Renderer(std::shared_ptr<Window> window,
			 std::shared_ptr<Camera> camera,
			 std::shared_ptr<ShaderProgram> shaderProgram);
	~Renderer();

	int setWindow(std::shared_ptr<Window> window);
	int setCamera(std::shared_ptr<Camera> camera);
	int setShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram);

	int render(std::shared_ptr<Node> node);
	int render(std::shared_ptr<Node> node, glm::mat4 trans);

private:
	int applyMatrix(glm::mat4 trans);
	int applyMaterial();

	std::shared_ptr<Window> mWindow;
	std::shared_ptr<Camera> mCamera;
	std::shared_ptr<ShaderProgram> mShaderProgram;
};
/*
void render(mgf_node_model *node, glm::mat4 trans = glm::mat4(1));
void render(mgf_node_model_instance *node, glm::mat4 = glm::mat4(1));
void render_single_node(mgf_node_model *node, glm::mat4 trans = glm::mat4(1));

bool apply_matrix(glm::mat4 mat, GLuint loc);
void apply_material(unsigned int material_index, mgf_data *data);
*/

}

#endif // MGF_RENDER
