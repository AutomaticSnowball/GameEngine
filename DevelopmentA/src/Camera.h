#pragma once

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "Chunk.h"


class Camera
{
public:
	Camera();
	Camera::Camera(float sens_Move, float sens_Look, int width, int height);
	glm::vec2 getRot();
	glm::vec3 getBlock();
	glm::vec2 getChunk();
	void setFOV(float _fov);
	float getFOV();
	glm::vec3 getPos();
	glm::vec3 getUp();
	glm::vec3 getFront();
	glm::vec3 getRight();
	void setPos(float x, float y, float z);
	void update(Chunk c);

	glm::mat4 viewMatrix();
	void KEY_INPUT(int KEY, float deltaTime);
	void MOUSE_MOVE(int x_offset, int y_offset);
private:
	float mouse_sensitivity = 0.05f;
	float move_sensitivity = 2.5f;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 c_Pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 c_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 c_Target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 c_Dir = glm::normalize(c_Pos - c_Target);
	glm::vec3 c_Right = glm::normalize(glm::cross(up, c_Dir));
	glm::vec3 c_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	Chunk currentChunk;
	float pitch = 0.0f;
	float yaw = 0.0f;
	bool gravity = true;
	bool fpsMode = true;
	unsigned int jumpTime;
	unsigned int modeSwap;
	float xBound = 0.55f;
	float yBound = -0.80f;
	float zBound = 0.55f;
	float fov = 45.0f;
};

