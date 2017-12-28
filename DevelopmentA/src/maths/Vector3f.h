#pragma once
#include <math.h>
class Vector3f
{
public:


	Vector3f();
	Vector3f(float x, float y, float z);
	void add(float scalar);
	void add(Vector3f b);
	void sub(float scalar);
	void sub(Vector3f b);
	void mul(float scalar);
	void div(float scalar);
	float getLength();
	float getLength(float origin_x, float origin_y, float origin_z);
	void normalise();
	float dot(Vector3f b);
	Vector3f cross(Vector3f b);
	float getX();
	float getY();
	float getZ();
	void setX(float _x);
	void setY(float _y);
	void setZ(float _z);
private:
	float x;
	float y;
	float z;
};

