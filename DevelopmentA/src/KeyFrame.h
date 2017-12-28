#pragma once

#include "JointTransform.h"

class KeyFrame
{
public:
	KeyFrame();
	KeyFrame(float timeStamp, std::map<std::string, JointTransform> jointKeyFrames);
	~KeyFrame();
	float getTimeStamp();
	std::map<std::string, JointTransform> getJointKeyFrames();
private:
	float timeStamp;
	std::map<std::string, JointTransform> pose;
};

