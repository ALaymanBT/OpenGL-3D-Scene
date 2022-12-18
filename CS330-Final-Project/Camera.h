#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Enum for camera movement values
enum movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	// Camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Angles for camera rotation
	float yaw;
	float pitch;

	// Speed and sensitivity
	float camSpeed;
	float sensitivity;

	// Constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

	// Returns lookat matrix
	glm::mat4 getViewMatrix();

	// Functions to process movement and change speed
	void keyboardMovement(movement direction, float deltaTime);
	void mouseInput(float xoffset, float yoffset);
	void scrollInput(float yoffset);

private:
	// Calculate front, right, and up camera vectors
	void updateCamVectors();
};

#endif