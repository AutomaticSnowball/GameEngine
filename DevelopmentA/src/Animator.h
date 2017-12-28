#pragma once

#include "Joint.h"
#include "Animation.h"

class Animator
{
public:
	Animator();
	Animator(Joint _rootJoint);
	void animate(Animation a);
	void update();
	Joint getRoot();
	void increaseTime();
	std::map<std::string, glm::mat4> calcCurrentPose();
	std::vector<KeyFrame> getPreAndNextFrame();
	float calcProgression(KeyFrame preFrame, KeyFrame nextFrame);
	std::map<std::string, glm::mat4> interpolatePoses(KeyFrame pre, KeyFrame next, float p);
	Joint applyPose(std::map<std::string, glm::mat4> c_pose, Joint j, glm::mat4 parentTransform);
	~Animator();
private:
	Joint rootJoint;
	Animation currentAnimation;
	float runTime;
	bool finished;
};

