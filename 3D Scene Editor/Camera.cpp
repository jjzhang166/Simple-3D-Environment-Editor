#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), lastX(START_X), lastY(START_Y) {
	this->position = pos;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), lastX(START_X), lastY(START_Y) {
	position = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);

	this->right = glm::normalize(glm::cross(front, worldUp));
	this->up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera::Movement direction, float deltaTime) {
	float speed = deltaTime * movementSpeed;
	switch (direction) {
	case(FORWARD):
		position.z += speed * front.z;
		position.x += speed * front.x;
		break;
	case(BACKWARD):
		position.z -= speed * front.z;
		position.x -= speed * front.x;
		break;
	case(LEFT):
		position -= speed * glm::normalize(glm::cross(front, up));
		break;
	case(RIGHT):
		position += speed * glm::normalize(glm::cross(front, up));
		break;
	default:
		LOG("ERROR: Camera movement input unrecognized.");
		break;
	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
	xOffset = xOffset * mouseSensitivity;
	yOffset = yOffset * mouseSensitivity;

	pitch += yOffset;
	yaw += xOffset;

	if (constrainPitch) {
		if (pitch >= 89.0f) {
			pitch = 89.0f;
		} else if (pitch <= -89.0f) {
			pitch = -89.0f;
		}
	}

	lastX -= xOffset;
	lastY -= yOffset;
	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
	const float sensitivity = 2.0f;
	zoom -= yOffset * sensitivity;
	if (zoom < 5.0f) {
		zoom = 5.0f;
	} else if (zoom > 45.0f) {
		zoom = 45.0f;
	}
}

