#pragma once

#include "PointLight.h"

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<PointLight> getPointLights();
	PointLight getPointLight(unsigned int ID);
	void setPointLight(unsigned int ID, PointLight p);
	void setPointLightAttrib(unsigned int ID, std::string attrib, glm::vec3 c);
	void deletePointLight(unsigned int ID);
	unsigned int getPLCount();
	void addPointLight(PointLight pl);
	std::vector<Object> getObjects();
	void addObject(Object o);
private:
	std::vector<PointLight> pointLights;
	unsigned int maxPointLights = 100;
	unsigned int pointLightCount;
	std::vector<Object> objects;
};

