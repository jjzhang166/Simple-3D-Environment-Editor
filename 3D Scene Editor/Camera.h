#pragma once

#ifndef CAMERA_H
#define CAMERA_H 1

#include "stdafx.hpp"

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 45.0f;
const float START_X = WINDOW_WIDTH / 2.0f;
const float START_Y = WINDOW_HEIGHT / 2.0f;

class Camera {
public:
	enum Movement {FORWARD, BACKWARD, LEFT, RIGHT};

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float lastX;
	float lastY;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 getViewMatrix();

	void processKeyboard(Camera::Movement direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);

private:
	void updateCameraVectors();
};

#endif