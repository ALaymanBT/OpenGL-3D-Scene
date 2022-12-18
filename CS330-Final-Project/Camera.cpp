#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {

	// Initialize variables to default values
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	camSpeed = 1.0f;
	sensitivity = 0.1f;
	this->position = position;
	worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;

	// Update the camera vectors
	updateCamVectors();
}

// Returns the lookat matrix
glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::keyboardMovement(movement direction, float deltaTime) {

	// Multiply speed by delta time to get a consistent speed
	float velocity = camSpeed * deltaTime;

	// Move forware or backward using camera front vector
	if (direction == FORWARD) {
		position += velocity * front;
	}
	if (direction == BACKWARD) {
		position -= velocity * front;
	}

	// Take the cross product of the front and top of the camera to get a right vector
	// then subtract or add to move left or right
	if (direction == LEFT) {
		position -= velocity * right;
	}
	if (direction == RIGHT) {
		position += velocity * right;
	}

	// Move up or down using camera up vector
	if (direction == DOWN) {
		position -= velocity * up;
	}
	if (direction == UP) {
		position += velocity * up;
	}
}
void Camera::mouseInput(float xoffset, float yoffset) {

	// Set a sensitivity and multiply the offsets
	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// Add the resulting offsets to yaw and pitch
	yaw += xoffset;
	pitch += yoffset;

	// Constrain pitch to a range of -89.0 to 89.0 to avoid camera flip
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	updateCamVectors();

}
void Camera::scrollInput(float yoffset) {
	// Update the camera speed

	camSpeed += yoffset;

	// Disallow going below 0.1 to avoid a negative camera speed
	if (camSpeed < 0.1) {
		camSpeed = 0.1;
	}
}
void Camera::updateCamVectors() {
	// Calculate the camera direction (front of camera)
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

	// Calculate right and up vectors
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}