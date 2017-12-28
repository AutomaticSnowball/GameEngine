#pragma once

#include "AnimationSkeleton.h"
#include "Animator.h"

class AnimatedModel
{
public:
	AnimatedModel();
	AnimatedModel(std::string path);
	AnimatedModel(AnimationSkeleton m);
	glm::mat4 getTransform();
	~AnimatedModel();
	AnimatedMesh getModel();
	std::vector<ATexture> getTexture();
	Joint getRootJoint();
	void animate(Animation a);
	void animate(unsigned int ID);
	void update();
	std::vector<glm::mat4> getJointTransforms();
	std::vector<glm::mat4> addJointsToArray(Joint root, std::vector<glm::mat4> r);
private:
	AnimationSkeleton skele;
	Joint jointRoot;
	unsigned int jointCount;
	Animator v_animator;
	std::vector<Animation> animations;
	glm::vec3 pos;
	glm::vec3 sca;
	std::map < std::string, float > rot;
};

