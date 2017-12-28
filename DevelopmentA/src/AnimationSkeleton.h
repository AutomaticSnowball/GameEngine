#pragma once

#include "AnimatedMesh.h"
#include "KeyFrame.h"
#include "Joint.h"
#include "Animation.h"
#include "stb_image.h"

class AnimationSkeleton
{
public:
	AnimationSkeleton();
	AnimationSkeleton(std::string path);
	void loadDae(std::string filePath);
	Animation getAnimation(unsigned int ID);
	std::vector<Animation> getAllAnimations();
	std::string getClosestParent(std::map<std::string, unsigned int> d, unsigned int pos);
	std::string getNameFromIdentifier(std::string input);
	void setKeyFrames(std::map<unsigned int, std::vector<glm::mat4>> _keyframes, std::vector<std::string> jointNames, std::vector<float> times);
	void setModelData(std::vector<glm::ivec3> i, std::vector<glm::vec3> p, std::vector<glm::vec3> n, std::vector<glm::vec2> t, std::vector<glm::vec3> j, std::vector<glm::vec3> w);
	ATexture genTexture(std::string path);
	std::vector<glm::vec3> getVec3FromString(std::string input, char seperator, bool xyz);
	std::vector<glm::vec2> getVec2FromString(std::string input, char seperator);
	std::vector<glm::ivec3> getIndicesFromString(std::string input, char seperator);
	std::vector<glm::mat4> getMat4FromString(std::string input, char seperator);
	std::vector<std::string> getStringFromString(std::string input, char seperator);
	std::vector<glm::ivec2> getIntVec2FromString(std::string input, char seperator);
	std::vector<float> getFloatFromString(std::string input, char seperator);
	std::vector<unsigned int> getIntFromString(std::string input, char seperator);
	std::string getDataFromIdentifier(std::string input);
	AnimatedMesh getMesh();
	std::vector<ATexture> getTexture();
	~AnimationSkeleton();
	Joint getRootJoint();
private:
	Joint rootJoint;
	AnimatedModelData modelData;
	AnimatedMesh mesh;
	std::vector<KeyFrame> keyFrames;
	std::vector<Animation> animations;
};

