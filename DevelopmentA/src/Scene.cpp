#include "Scene.h"

Scene::Scene()
{
	pointLightCount = 0;
	for (int i = 0; i < 100; i++) {
		pointLights[i] = PointLight(glm::vec3(i,0,-5),glm::vec3((float)1/i,(float)1/i,1.0f));
		if (i > 3) {
			pointLights[i].setActive(false);

		}
		else {
			pointLights[i].setActive(true);
		}
		pointLightCount++;
	}
}


Scene::~Scene()
{
}

PointLight Scene::getPointLight(unsigned int ID) {
	if (ID > 99 || ID < 0) {
		std::cout << "There is a limit of 100 point lights in the scene!" << std::endl;
	}
	else {
		return pointLights[ID];
	}
}

void Scene::setPointLight(unsigned int ID, PointLight p) {
	pointLights[ID] = p;
}

void Scene::setPointLightAttrib(unsigned int ID, std::string attrib, glm::vec3 c) {
	if (attrib == "diffuse") {
		pointLights[ID].setDiffuse(c);
	}
}

void Scene::deletePointLight(unsigned int ID) {
	if (ID > 99 || ID < 0) {
		std::cout << "There is a limit of 100 point lights in the scene!" << std::endl;
	}
	else {
		pointLights[ID].setActive(false);
		pointLightCount--;
	}
}

unsigned int Scene::getPLCount() {
	return pointLightCount;
}

std::vector<Object> Scene::getObjects() {
	return objects;
}

void Scene::addObject(Object o) {
	objects.push_back(o);
}