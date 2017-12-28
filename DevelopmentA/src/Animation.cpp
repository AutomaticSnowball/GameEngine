#include "Animation.h"

Animation::Animation() {

}


Animation::Animation(float length_seconds, std::vector<KeyFrame> frames)
{
	length = length_seconds;
	keyframes = frames;
}

float Animation::getLength() {
	return length;
}

std::vector<KeyFrame> Animation::getKeyFrames() {
	return keyframes;
}


Animation::~Animation()
{
}
