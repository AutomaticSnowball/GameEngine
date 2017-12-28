#include "Vector4f.h"

Vector4f::Vector4f()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}
Vector4f::Vector4f(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Vector4f::add(float scalar) {
	x += scalar;
	y += scalar;
	z += scalar;
}
void Vector4f::add(Vector4f b) {
	x += b.getX();
	y += b.getY();
	z += b.getZ();
}

void Vector4f::sub(float scalar) {
	x -= scalar;
	y -= scalar;
	z -= scalar;
}
void Vector4f::sub(Vector4f b) {
	x -= b.getX();
	y -= b.getY();
	z -= b.getZ();
}

void Vector4f::mul(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vector4f::div(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

float Vector4f::getLength() {
	return getLength(0, 0, 0);
}
float Vector4f::getLength(float origin_x, float origin_y, float origin_z) {
	float _x = x - origin_x;
	float _y = y - origin_y;
	float _z = z - origin_z;
	float xz = sqrt((_x*_x) + (_z*_z));
	float ybase = sqrt((xz*xz) + (_y*_y));
	return ybase;
}

void Vector4f::normalise() {
	float length = getLength();
	if (length > 0) {
		div(length);
	}
}

float Vector4f::dot(Vector4f b) {
	float _x = x * b.getX();
	float _y = y * b.getY();
	float _z = z * b.getZ();
	return _x + _y + _z;
}

Vector4f Vector4f::cross(Vector4f b) {
	float _x = ((y*b.z) - (z*b.y));
	float _y = ((z*b.x) - (x*b.z));
	float _z = ((x*b.y) - (y*b.x));
	return Vector4f(_x, _y, _z, b.getW());
}

float Vector4f::getX() {
	return x;
}
float Vector4f::getY() {
	return y;
}
float Vector4f::getZ() {
	return z;
}
float Vector4f::getW() {
	return w;
}

void Vector4f::setX(float _x) {
	x = _x;
}
void Vector4f::setY(float _y) {
	y = _y;
}
void Vector4f::setZ(float _z) {
	z = _z;
}
void Vector4f::setW(float _w) {
	w = _w;
}
void Vector4f::addW(float _w) {
	w = w + _w;
}