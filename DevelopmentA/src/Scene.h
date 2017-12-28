#pragma once

#include "PointLight.h"

class Scene
{
public:
	Scene();
	~Scene();
	PointLight getPointLight(unsigned int ID);
	void setPointLight(unsigned int ID, PointLight p);
	void setPointLightAttrib(unsigned int ID, std::string attrib, glm::vec3 c);
	void deletePointLight(unsigned int ID);
	unsigned int getPLCount();
	std::vector<Object> getObjects();
	void addObject(Object o);
private:
	PointLight pointLights[100];
	unsigned int pointLightCount;
	std::vector<Object> objects;
};

