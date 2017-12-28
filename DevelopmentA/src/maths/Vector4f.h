#pragma once
#include <math.h>
class Vector4f
{
public:
	Vector4f();
	Vector4f(float _x, float _y, float _z, float _w);
	void add(float scalar);
	void add(Vector4f b);
	void sub(float scalar);
	void sub(Vector4f b);
	void mul(float scalar);
	void div(float scalar);
	float getLength();
	float getLength(float origin_x, float origin_y, float origin_z);
	void normalise();
	float dot(Vector4f b);
	Vector4f cross(Vector4f b);
	float getX();
	float getY();
	float getZ();
	float getW();
	void setX(float _x);
	void setY(float _y);
	void setZ(float _z);
	void setW(float _w);
	void addW(float _w);
private:
	float x;
	float y;
	float z;
	float w;
};

