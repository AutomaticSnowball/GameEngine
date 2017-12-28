#pragma once
#include "Vector4f.h"
#include <math.h>

class Matrix4
{
public:
	float matrix[4][4];
	Matrix4();
	Matrix4(float values[4][4]);
	void Add(float scalar);
	void Add(Matrix4 b);
	void Sub(Matrix4 b);
	void Mul(Matrix4 b);
	Vector4f Mul(Vector4f b);
	void Mul(float scalar);
	void Scale(float scalar_x, float scale_y, float scale_z);
	void Translate(float x, float y, float z);
	void Rotate(float angle, int axis); //Axis 1 = x, 2 = y, 3 = z.
	float getValue(int x, int y);
};

