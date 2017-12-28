#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "GLM/glm.hpp"
#include "GLM/gtc/quaternion.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtx/matrix_decompose.hpp"
#include <vector>
#include <map>


class JointTransform
{
public:
	JointTransform();
	JointTransform(glm::vec3 _pos, glm::quat _rot);

	JointTransform(glm::mat4 _m);

	glm::mat4 getLocalTransform();

	JointTransform interpolate(JointTransform a, JointTransform b, float time);

	glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float time);
	
	~JointTransform();
private:
	glm::vec3 pos;
	glm::quat rot;
};

