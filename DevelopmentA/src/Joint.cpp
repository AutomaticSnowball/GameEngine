#include "Joint.h"

Joint::Joint() {

}

Joint::Joint(int _index, std::string _name, unsigned int bindType, glm::mat4 localbind)
{
	index = _index;
	name = _name;
	if (bindType == 0) {
		bind_loc_transform = localbind;
		hasInv = false;

	}
	else {
		bind_inv_transform = localbind;
		hasInv = true;
	}

}

Joint::~Joint()
{
}

void Joint::addChild(Joint j) {
	children.push_back(j);
}

glm::mat4 Joint::getAnimatedTransform() {
	return animated_transform;
}

void Joint::setAnimationTransform(glm::mat4 transform) {
	animated_transform = transform;
}

glm::mat4 Joint::getBindInverseTransform() {
	return bind_inv_transform;
}

void Joint::calcBindInverseTransform(glm::mat4 parentBind) {
	glm::mat4 bindTransform = glm::mat4(parentBind * bind_loc_transform);
	glm::mat4 inverseBind = glm::inverse(bindTransform);
	bind_inv_transform = inverseBind;
	unsigned int index = 0;
	for each (Joint j in children) {
		j.calcBindInverseTransform(bindTransform);
		children[index] = j;
		index++;
	}
	hasInv = true;
}

std::vector<Joint> Joint::getChildren()
{
	std::vector<Joint> childs;
	for each (Joint j in children) {
		std::vector < Joint> a = j.getChildren();
		childs.insert(childs.end(), a.begin(), a.end());
	}
	childs.push_back(*this);
	return childs;
}

Joint Joint::getChild(unsigned int _index) {
	std::vector<Joint> j;
	j = getChildren();
	for each (Joint joint in j) {
		if (joint.index == _index) {
			return joint;
		}
	}
	Joint defaultJoint;
	return defaultJoint;
}

Joint Joint::getChild(std::string _name) {
	std::vector<Joint> j;
	j = getChildren();
	for each (Joint joint in j) {
		if (joint.name == _name) {
			return joint;
		}
	}
	Joint defaultJoint;
	return defaultJoint;
}

bool Joint::setChild(std::string _name, Joint c) {
	if (name == _name) {
		addChild(c);
		return true;
	}
	unsigned int ind = 0;
	for each (Joint joint in children) {
		joint.setChild(_name, c);
		children[ind] = joint;
		ind++;
	}
	return false;
}

unsigned int Joint::getCount() {
	return getChildren().size();
}

bool Joint::getHasInv()
{
	return hasInv;
}
