#pragma once

#include "KeyFrame.h"

class Animation
{
public:
	Animation();
	Animation(float length_seconds, std::vector<KeyFrame> frames);
	float getLength();
	std::vector<KeyFrame> getKeyFrames();
	~Animation();
private:
	float length;
	std::vector<KeyFrame> keyframes;

};

