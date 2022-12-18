#ifndef LIGHTS_H
#define LIGHTS_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Lights
{
public:
	// Constructor
	Lights();

	// Configuration functions
	void setPosition(float x, float y, float z);
	void setColor(float r, float g, float b);
	void setIntensity(float brightness);
	float getIntensity();
	glm::vec3 getPosition();
	glm::vec3 getColor();

private:

	// Variables for light color and position. To be passed into a shader.
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	float intensity;

};

#endif