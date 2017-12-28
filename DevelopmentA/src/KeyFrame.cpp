#include "KeyFrame.h"


KeyFrame::KeyFrame() {

}

KeyFrame::KeyFrame(float _timeStamp, std::map<std::string, JointTransform> jointKeyFrames)
{
	timeStamp = _timeStamp;
	pose = jointKeyFrames;
}

KeyFrame::~KeyFrame()
{
}

float KeyFrame::getTimeStamp() {
	return timeStamp;
}

std::map<std::string, JointTransform> KeyFrame::getJointKeyFrames() {
	return pose;
}