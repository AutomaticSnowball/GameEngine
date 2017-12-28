#include "JointTransform.h"

JointTransform::JointTransform() {
	pos = glm::vec3(0.0f,0.0,0.0f);
	rot = glm::quat();
}

JointTransform::JointTransform(glm::vec3 _pos, glm::quat _rot)
{
	pos = _pos;
	rot = _rot;
}

JointTransform::JointTransform(glm::mat4 _m) {
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::decompose(_m, scale, rot, pos, skew, persp);
	rot = glm::conjugate(rot);
}

glm::mat4 JointTransform::getLocalTransform() {
	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, pos);
	m = glm::mat4(m * glm::mat4_cast(rot));
	return m;
}

JointTransform JointTransform::interpolate(JointTransform a, JointTransform b, float time) {
	glm::vec3 position = interpolate(a.pos, b.pos, time);
	glm::quat rotation = glm::slerp(a.rot, b.rot, time);
	return JointTransform(position, rotation);
}

glm::vec3 JointTransform::interpolate(glm::vec3 a, glm::vec3 b, float time) {
	float x = a.x + (b.x - a.x) * time;
	float y = a.y + (b.y - a.y) * time;
	float z = a.z + (b.z - a.z) * time;
	return glm::vec3(x, y, z);
}


JointTransform::~JointTransform()
{
}
