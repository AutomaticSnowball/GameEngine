#include "Camera.h"

Camera::Camera()
{
	mouse_sensitivity = 0.05f;
	move_sensitivity = 2.5f;
	c_Pos = glm::vec3(0.0f, 0.0f, 3.0f);
	c_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	c_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0;
	pitch = 0;
	fov = 45.0f;
}
Camera::Camera(float sens_Move, float sens_Look, int width, int height) {
	mouse_sensitivity = sens_Look;
	move_sensitivity = sens_Move;
}

void Camera::update(Chunk c) {
	currentChunk = c;
	float updateRate = 1 / 60.0f;
	modeSwap++;
	if (jumpTime > 0) {
		jumpTime++;
		if (jumpTime < 20) {
			c_Pos += glm::vec3(0.0f, cos(glm::radians((float)jumpTime)) / 12, 0.0f);
		}
		else {
			if (c.getBlock(getBlock().x, getBlock().y - 2, getBlock().z) == 0) {
				c_Pos += glm::vec3(0.0f, -(9.81 * updateRate), 0.0f);
			}
		}

		if (jumpTime == 40) {
			jumpTime = 0;
		}
	}
	if (gravity) {
		if (jumpTime == 0) {
			if (c.getBlock(getBlock().x, getBlock().y - 2, getBlock().z) == 0) {
				c_Pos += glm::vec3(0.0f, -(9.81 * updateRate), 0.0f);
			}
		}
	}
}

void Camera::setPos(float x, float y, float z) {
	c_Pos = glm::vec3(x, y, z);
}

glm::vec3 Camera::getBlock() {
	glm::vec3 blockPos = c_Pos;
	blockPos.x = round(blockPos.x);
	blockPos.y = round(blockPos.y) - 1;
	blockPos.z = round(blockPos.z);
	if (blockPos.x == -0) {
		blockPos.x = 0;
	}
	if (blockPos.z == -0) {
		blockPos.z = 0;
	}
	blockPos.x = round(blockPos.x) + (16 * -currentChunk.getStart()[0]);
	blockPos.z = round(blockPos.z) + (16 * -currentChunk.getStart()[1]);
	return blockPos;
}

glm::vec2 Camera::getChunk() {
	glm::vec2 pos;
	pos.x =(c_Pos.x + 0.5);
	pos.y =(c_Pos.z + 0.5);
	pos.x = pos.x / 16;
	pos.y = pos.y / 16;
	if (pos.x == -0) { pos.x = -1; }
	if (pos.y == -0) { pos.y = -1; }

	pos.x = floor(pos.x);
	pos.y = floor(pos.y);
	return pos;
}

void Camera::setFOV(float _fov) {
	fov = _fov;
}

float Camera::getFOV() {
	return fov;
}

glm::vec2 Camera::getRot() {
	return glm::vec2(yaw, pitch);
}

glm::vec3 Camera::getPos() {
	return c_Pos;
}
glm::vec3 Camera::getFront() {
	return c_Front;
}
glm::vec3 Camera::getUp() {
	return c_Up;
}
glm::vec3 Camera::getRight() {
	return c_Right;
}

void Camera::KEY_INPUT(int KEY, float deltaTime) {
	float c_Speed = 6.5f * deltaTime;

	if (KEY == 400) {
		if (modeSwap > 10) {
			fpsMode = !fpsMode;
			gravity = !gravity;
			modeSwap = 0;
		}

	}

	if (KEY == 5) {
		//Up
		if (fpsMode) {
			if (jumpTime == 0) {
				if (currentChunk.getBlock(c_Pos.x, c_Pos.y - 2 + yBound, c_Pos.z) != 0) {
					jumpTime += 1;
				}
			}
		} else {
			c_Pos += c_Speed * glm::vec3(0.0f, 1.0f, 0.0f);
		}

	}
	if (KEY == 6) {
		//Down
		c_Pos += c_Speed * glm::vec3(0.0f, -1.0f, 0.0f);
	}
	if (KEY == 1) {
		//Forward.
		if (fpsMode) {
			glm::vec3 newPos = c_Pos + (c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z));
			c_Pos += c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z);
			if (currentChunk.getBlock(floor(getBlock() + newPos).x, floor(getBlock() + newPos).y, floor(getBlock() + newPos).z) == 0) {
				//c_Pos += c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z);
			}
			else {
				//c_Pos += glm::vec3(0.0f, 5.0f, 0.0f);
			}
		}
		else {
			c_Pos += c_Speed * c_Front;
		}
	}
	else if (KEY == 2) {
		//Backward
		if (fpsMode) {
			c_Pos -= c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z);
		}
		else {
			c_Pos -= c_Speed * c_Front;
		}
		
	}
	if (KEY == 3) {
		//Left
		c_Pos -= c_Speed * glm::normalize(glm::cross(c_Front, c_Up));
	}
	else if (KEY == 4) {
		//Right
		c_Pos += c_Speed * glm::normalize(glm::cross(c_Front, c_Up));
	}
}


void Camera::MOUSE_MOVE(int x_offset, int y_offset) {
	float sensitivity = 0.05f;
	x_offset *= sensitivity;
	y_offset *= sensitivity;

	yaw = glm::mod(yaw + x_offset, 360.0f);
	pitch += y_offset;
	if (pitch > 89.0f) { pitch = 89.0f; }
	if (pitch < -89.0f) { pitch = -89.0f; }
	glm::vec3 t_front;
	t_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	t_front.y = sin(glm::radians(pitch));
	t_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	c_Front = glm::normalize(t_front);
}

glm::mat4 Camera::viewMatrix() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(c_Pos, //Camera position
		c_Pos + c_Front, //Target
		c_Up); //Up vector
	return view;
}