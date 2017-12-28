#include "AnimatedModel.h"


AnimatedModel::AnimatedModel() {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	rot["x"] = 0.0f;
	rot["y"] = 0.0f;
	rot["z"] = 0.0f;
}

AnimatedModel::AnimatedModel(std::string path) {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	rot["x"] = 0.0f;
	rot["y"] = 0.0f;
	rot["z"] = 0.0f;
	AnimationSkeleton m(path);
	skele = m;
	jointRoot = m.getRootJoint();
	jointCount = m.getRootJoint().getCount();
	if (!jointRoot.getHasInv()) {
		jointRoot.calcBindInverseTransform(glm::mat4(1.0f));
	}
	v_animator = Animator(jointRoot);
	animations = m.getAllAnimations();
}

AnimatedModel::AnimatedModel(AnimationSkeleton m)
{
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	rot["x"] = 0.0f;
	rot["y"] = 0.0f;
	rot["z"] = 0.0f;
	skele = m;
	jointRoot = m.getRootJoint();
	jointCount = m.getRootJoint().getCount();
	if (!jointRoot.getHasInv()) {
		jointRoot.calcBindInverseTransform(glm::mat4(1.0f));
	}
	v_animator = Animator(jointRoot);
	animations = m.getAllAnimations();
}

glm::mat4 AnimatedModel::getTransform() {
	glm::mat4 result = glm::mat4(1.0f);
	result = glm::scale(result, sca);
	result = glm::translate(result, pos);
	result = glm::rotate(result, glm::radians(rot["x"]), glm::vec3(1.0f, 0.0f, 0.0f));
	result = glm::rotate(result, glm::radians(rot["y"]), glm::vec3(0.0f, 1.0f, 0.0f));
	result = glm::rotate(result, glm::radians(rot["z"]), glm::vec3(1.0f, 0.0f, -1.0f));
	return result;
}

AnimatedMesh AnimatedModel::getModel() {
	return skele.getMesh();
}

std::vector<ATexture> AnimatedModel::getTexture() {
	return skele.getTexture();
}

Joint AnimatedModel::getRootJoint() {
	return jointRoot;
}

void AnimatedModel::animate(Animation a) {
	v_animator.animate(a);
}

void AnimatedModel::animate(unsigned int ID) {
	v_animator.animate(animations[ID]);
}

void AnimatedModel::update() {
	v_animator.update();
	jointRoot = v_animator.getRoot();
}

//Adds all joint transforms to the array.
std::vector<glm::mat4> AnimatedModel::getJointTransforms() {
	std::vector<glm::mat4> r;
	if (r.size() == 0) {
		for (int i = 0; i < jointRoot.getChildren().size(); i++) {
			r.push_back(glm::mat4(1.0f));
		}
	}
	r = addJointsToArray(jointRoot, r);
	return r;
}

std::vector<glm::mat4> AnimatedModel::addJointsToArray(Joint root, std::vector<glm::mat4> r) {
	r[root.index] = root.getAnimatedTransform();
	for each (Joint j in root.children) {
		r = addJointsToArray(j, r);
	}
	return r;
}




AnimatedModel::~AnimatedModel()
{

}