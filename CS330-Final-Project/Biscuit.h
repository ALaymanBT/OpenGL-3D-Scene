#ifndef BISCUIT_H
#define BISCUIT_H
#include "Mesh.h"
#include "Shader.h"


class Biscuit : public Mesh
{
public:
	Biscuit();
	Cylinder cylinder;
	Mesh circleTop;
	Mesh circleBottom;

	void Draw(Shader& shader, glm::mat4 proj, glm::mat4 view);

private:

	void genBiscuit();

};

#endif