#include "Matrix4.h"

Matrix4::Matrix4()
{
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			matrix[r][c] = 0.0f;
		}
	}
}

Matrix4::Matrix4(float values[4][4]) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			matrix[r][c] = values[r][c];
		}
	}
}

void Matrix4::Add(float scalar) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			matrix[r][c] += scalar;
		}
	}
}

void Matrix4::Add(Matrix4 b) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			matrix[r][c] += b.matrix[r][c];
		}
	}
}
void Matrix4::Sub(Matrix4 b) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			matrix[r][c] -= b.matrix[r][c];
		}
	}
}
void Matrix4::Mul(float scalar) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			matrix[r][c] *= scalar;
		}
	}
}

void Matrix4::Mul(Matrix4 b) {
	float result[4][4];
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			float sum = 0;
			for (int i = 0; i < 4; i++) {
				sum += matrix[r][i] * b.matrix[i][c];
			}
			result[r][c] = sum;
		}
	}
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			matrix[r][c] = result[r][c];
		}
	}
}

Vector4f Matrix4::Mul(Vector4f b) {
	Vector4f result = b;
	float sums[4];
	for (int i = 0; i < 4; i++) {
		sums[i] = 0.0f;
	}
	for (int r = 0; r < 4; r++) {
		sums[r] += matrix[r][0] * b.getX();
		sums[r] += matrix[r][1] * b.getY();
		sums[r] += matrix[r][2] * b.getZ();
		sums[r] += matrix[r][3] * b.getW();
	}
	result.setX(sums[0]);
	result.setY(sums[1]);
	result.setZ(sums[2]);
	result.setW(sums[3]);
	return result;
}

void Matrix4::Scale(float scalar_x, float scale_y, float scale_z) {
	matrix[0][0] *= scalar_x;
	matrix[1][1] *= scale_y;
	matrix[2][2] *= scale_z;
}

void Matrix4::Translate(float x, float y, float z) {
	matrix[0][3] += x;
	matrix[1][3] += y;
	matrix[2][3] += z;
}

void Matrix4::Rotate(float angle, int axis) {
	float radian = angle * (3.14159265359 / 180.0f);
	if (axis == 1) {
		matrix[1][1] *= cos(angle);
		matrix[1][2] *= -sin(angle);
		matrix[2][1] *= sin(angle);
		matrix[2][2] *= cos(angle);
	} 
	if (axis == 2) {
		matrix[0][0] *= cos(angle);
		matrix[0][2] *= sin(angle);
		matrix[2][0] *= -sin(angle);
		matrix[2][2] *= cos(angle);
	}
	if (axis == 3) {
		matrix[0][0] *= cos(angle);
		matrix[0][1] *= -sin(angle);
		matrix[1][0] *= sin(angle);
		matrix[1][1] *= cos(angle);
	}
}

float Matrix4::getValue(int x, int y) {
	return matrix[x][y];
}