#ifndef MUG_H
#define MUG_H
#include "Mesh.h"
#include "Shader.h"


class Mug : public Mesh
{
public:
	Mug();
	Cylinder cylinder;
	Torus torus;
	Mesh circle;

	void Draw(Shader& shader, glm::mat4 proj, glm::mat4 view);

private:

	void genMug();

};

#endif