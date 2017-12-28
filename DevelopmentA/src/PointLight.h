#pragma once
#include "Object.h"

class PointLight : public Object
{
public:
	PointLight();
	PointLight(glm::vec3 p);
	PointLight(glm::vec3 p, glm::vec3 diffuse);
	~PointLight();
	bool isActive();
	void setActive(bool a);
	void setDiffuse(glm::vec3 c);
	void setAmbient(glm::vec3 c);
	void setSpecular(glm::vec3 c);
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	glm::vec3 getAmbient();
private:
	glm::vec3 colour_diff;
	glm::vec3 colour_spec;
	glm::vec3 colour_amb;
	bool active;
};

