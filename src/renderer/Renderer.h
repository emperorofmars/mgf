/*
**	Author:		Martin Schwarz
**	Name:		Renderer.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_RENDERER_H
#define MGF_RENDERER_H

#include "../Include.h"
#include "../util/Window.h"
#include "../util/ShaderProgram.h"
#include "../util/Camera.h"
#include "../data/Data.h"

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

	int addLight(Light data, glm::mat4 transform);
	int removeLight(Light data);
	int clearLights();
	int drawMesh(Mesh data, glm::mat4 transform);

	bool good();

private:
	int applyMatrix(glm::mat4 data);
	int applyMaterial(Material data);

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

} // mgf

#endif // MGF_RENDERER_H
