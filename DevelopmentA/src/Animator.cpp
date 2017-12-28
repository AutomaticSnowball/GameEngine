#include "Animator.h"

Animator::Animator() {

}

Animator::Animator(Joint _rootJoint)
{
	rootJoint = _rootJoint;
}

void Animator::animate(Animation a) {
	finished = false;
	runTime = 0.5;
	currentAnimation = a;
}

void Animator::update() {
	if (finished == true) {
		return;
	}
	else {
		increaseTime();
		std::map<std::string, glm::mat4> pose = calcCurrentPose();
		rootJoint = applyPose(pose, rootJoint, glm::mat4(1.0f));
	}
}

Joint Animator::getRoot() {
	return rootJoint;
}

void Animator::increaseTime()  {
	if (runTime > 0.8) {
		runTime = 0;
	}
	runTime += (float)(0.01);
}

std::map<std::string, glm::mat4> Animator::calcCurrentPose() {
	std::vector<KeyFrame> frames = getPreAndNextFrame();
	float progression = calcProgression(frames[0], frames[1]);
	return interpolatePoses(frames[0], frames[1], progression);
}

std::vector<KeyFrame> Animator::getPreAndNextFrame() {
	std::vector<KeyFrame> frames = currentAnimation.getKeyFrames();
	KeyFrame preFrame = frames[0];
	KeyFrame nextFrame = frames[0];
	for (unsigned int i = 1; i < frames.size(); i++) {
		nextFrame = frames[i];
		if (nextFrame.getTimeStamp() > runTime) {
			break;
		}
		preFrame = frames[i];
	}
	frames.clear();
	frames.push_back(preFrame);
	frames.push_back(nextFrame);
	return frames;
}

float Animator::calcProgression(KeyFrame preFrame, KeyFrame nextFrame) {
	//Finds how far in the frame is. ie a multiplier between 0 and 1.
	float time = nextFrame.getTimeStamp() - preFrame.getTimeStamp();
	float currentTime = runTime - preFrame.getTimeStamp();
	return currentTime / time;
}

std::map<std::string, glm::mat4> Animator::interpolatePoses(KeyFrame pre, KeyFrame next, float p) {
	std::map<std::string, glm::mat4> result;
	for (auto const& r : pre.getJointKeyFrames()) {
		JointTransform preTransform = pre.getJointKeyFrames().at(r.first);
		JointTransform nextTransform = next.getJointKeyFrames().at(r.first);
		JointTransform currentT = currentT.interpolate(preTransform, nextTransform, p);
		result.insert(std::pair<std::string, glm::mat4>(r.first, currentT.getLocalTransform()));
	}
	return result;
}

Joint Animator::applyPose(std::map<std::string,glm::mat4> c_pose, Joint j, glm::mat4 parentTransform) {
	glm::mat4 localTransform = c_pose.at(j.name);
	glm::mat4 transform = glm::mat4(parentTransform * localTransform); //Model space.

	unsigned int index = 0;
	for each (Joint c_joint in j.children) {
		j.children[index] = applyPose(c_pose, c_joint, transform);
		index++;
	}

	transform = glm::mat4(transform * j.getBindInverseTransform());
	j.setAnimationTransform(transform);
	return j;
}

Animator::~Animator()
{
}
