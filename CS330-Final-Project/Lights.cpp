#include "Lights.h"

Lights::Lights() {

	lightPos = glm::vec3(0.0f);
	lightColor = glm::vec3(0.0f);
	intensity = 1.0;

}

// Update the position of the light
void Lights::setPosition(float x, float y, float z) {

	this->lightPos = glm::vec3(x, y, z);
}

// Update the color of the light
void Lights::setColor(float r, float g, float b) {

	this->lightColor = glm::vec3(r, g, b);
}

// Return the position of the light
glm::vec3 Lights::getPosition() {

	return this->lightPos;
}

// Return the color of the light
glm::vec3 Lights::getColor() {

	return this->lightColor;
}

void Lights::setIntensity(float brightness) {

	// Cap intensity at 1.0
	if (brightness > 1.0) {
		this->intensity = 1.0;
	}
	else if (brightness < 0.0) {
		this->intensity = 0.0;
	}
	else {
		this->intensity = brightness;
	}
}
float Lights::getIntensity() {
	return intensity;
}