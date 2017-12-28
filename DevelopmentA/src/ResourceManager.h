#pragma once
#include "Model.h"
#include "World.h"
#include "Camera.h"
#include "Scene.h"
#include "AnimatedModel.h"
#include "Mesh.h"


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	World getWorld(unsigned int ID);
	Model getModel(unsigned int ID);
	void add(World w);
	void add(Model m);
	void reset();
	void setMatrices(glm::mat4 _view, glm::mat4 _proj);
	void renderAnimated(Scene s);
	void render(Scene s);
	Chunk getChunk(unsigned int ID, int x, int z);
	Chunk getViewChunk(unsigned int ID, glm::vec2 p, glm::vec3 pos);
	unsigned int getBlock(unsigned int ID, int x, int y, int z);
	void drawBlock(Shader s, unsigned int ID, glm::vec3 pos);
	Camera getCamera();
	void setCamera(Camera c);
private:
	std::vector<Shader> shaders;
	std::vector<Model> models;
	std::vector<AnimatedModel> animated;
	std::vector<World> worlds;
	Camera cam;
	glm::mat4 w_view;
	glm::mat4 w_projection;
};

