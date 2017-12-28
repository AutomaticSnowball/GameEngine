#include "Object.h"

Object::~Object() {

}

Object::Object() {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	rot["x"] = 0.0f;
	rot["y"] = 0.0f;
	rot["z"] = 0.0f;
	model = Model();
}

Object::Object(glm::vec3 p) {
	pos = p;
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	rot["x"] = 0.0f;
	rot["y"] = 0.0f;
	rot["z"] = 0.0f;
	model = Model();
}

Object::Object(Model m) {
	model = m;
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	rot["x"] = 0.0f;
	rot["y"] = 0.0f;
	rot["z"] = 0.0f;
}

glm::mat4 Object::getTransform() {
	glm::mat4 result = glm::mat4(1.0f);
	result = glm::scale(result, sca);
	result = glm::translate(result, pos);
	result = glm::rotate(result, glm::radians(rot["x"]), glm::vec3(1.0f, 0.0f, 0.0f));
	result = glm::rotate(result, glm::radians(rot["y"]), glm::vec3(0.0f, 1.0f, 0.0f));
	result = glm::rotate(result, glm::radians(rot["z"]), glm::vec3(1.0f, 0.0f, -1.0f));
	return result;
}

glm::vec3 Object::getPos() {
	return pos;
}

void Object::setModel(Model m) {
	model = m;
}

void Object::draw(Shader s) {
	model.draw(s);
}

Model Object::getModel() {
	return model;
}

void Object::rotate(std::string axis, float degree) {
	rot[axis] += degree;
}

void Object::move(glm::vec3 amount) {
	pos += amount;
}

void Object::scale(glm::vec3 scalars) {
	sca *= scalars;
}

void Object::moveTo(glm::vec3 p) {
	pos = p;
}