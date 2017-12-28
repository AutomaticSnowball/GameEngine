#include "Vector3f.h"



Vector3f::Vector3f()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
Vector3f::Vector3f(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

void Vector3f::add(float scalar) {
	x += scalar;
	y += scalar;
	z += scalar;
}
void Vector3f::add(Vector3f b) {
	x += b.getX();
	y += b.getY();
	z += b.getZ();
}

void Vector3f::sub(float scalar) {
	x -= scalar;
	y -= scalar;
	z -= scalar;
}
void Vector3f::sub(Vector3f b) {
	x -= b.getX();
	y -= b.getY();
	z -= b.getZ();
}

void Vector3f::mul(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vector3f::div(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

float Vector3f::getLength() {
	return getLength(0, 0, 0);
}
float Vector3f::getLength(float origin_x, float origin_y, float origin_z) {
	float _x = x - origin_x;
	float _y = y - origin_y;
	float _z = z - origin_z;
	float xz = sqrt((_x*_x) + (_z*_z));
	float ybase = sqrt((xz*xz) + (_y*_y));
	return ybase;
}

void Vector3f::normalise() {
	float length = getLength();
	if (length > 0) {
		div(length);
	}
}

float Vector3f::dot(Vector3f b) {
	float _x = x * b.getX();
	float _y = y * b.getY();
	float _z = z * b.getZ();
	return _x + _y + _z;
}

Vector3f Vector3f::cross(Vector3f b) {
	float _x = ((y*b.z) - (z*b.y));
	float _y = ((z*b.x) - (x*b.z));
	float _z = ((x*b.y) - (y*b.x));
	return Vector3f(_x, _y, _z);
}

float Vector3f::getX() {
	return x;
}
float Vector3f::getY() {
	return y;
}
float Vector3f::getZ() {
	return z;
}

void Vector3f::setX(float _x) {
	x = _x;
}
void Vector3f::setY(float _y) {
	y = _y;
}
void Vector3f::setZ(float _z) {
	z = _z;
}