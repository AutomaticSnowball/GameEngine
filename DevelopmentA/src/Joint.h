#pragma once

#include <string>
#include <vector>
#include "GLM\glm.hpp"

class Joint
{
public:
	Joint();
	Joint(int _index, std::string _name, unsigned int type, glm::mat4 localbind);
	~Joint();
	void addChild(Joint j);
	glm::mat4 getAnimatedTransform();
	void setAnimationTransform(glm::mat4 transform);
	glm::mat4 getBindInverseTransform();
	void calcBindInverseTransform(glm::mat4 parentBind);
	unsigned int index;
	std::vector<Joint> getChildren();
	Joint getChild(unsigned int index);
	Joint getChild(std::string _name);
	bool setChild(std::string _name, Joint c);
	unsigned int getCount();
	std::string name;
	std::vector<Joint> children;
	bool getHasInv();
private:
	glm::mat4 animated_transform;
	glm::mat4 bind_loc_transform;
	glm::mat4 bind_inv_transform;
	bool hasInv = false;
};

