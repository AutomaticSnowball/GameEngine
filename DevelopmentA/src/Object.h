#pragma once
#include "Model.h"
#include <map>

class Object
{
public:
	Object();
	Object(glm::vec3 p);
	~Object();
	Object(Model m);
	glm::mat4 getTransform();
	glm::vec3 getPos();
	void setModel(Model m);
	Model getModel();
	void rotate(std::string axis, float degree);
	void move(glm::vec3 amount);
	void scale(glm::vec3 scalars);
	void moveTo(glm::vec3 p);
	void draw(Shader s);
private:
	Model model;
	glm::vec3 pos;
	glm::vec3 sca;
	std::map < std::string , float > rot;
};

