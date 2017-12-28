#pragma once
#include "Model.h"
#include "Camera.h"
#include "Scene.h"
#include "AnimatedModel.h"
#include "Mesh.h"


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	Model getModel(unsigned int ID);
	void add(Model m);
	void reset();
	void setMatrices(glm::mat4 _view, glm::mat4 _proj);
	void end();
	void renderAnimated(Scene s);
	void render(Scene s);
	void drawBlock(Shader s, unsigned int ID, glm::vec3 pos);
	Camera getCamera();
	void setCamera(Camera c);
private:
	std::vector<Shader> shaders;
	std::vector<Model> models;
	std::vector<AnimatedModel> animated;
	Camera cam;
	glm::mat4 w_view;
	glm::mat4 w_projection;
};

