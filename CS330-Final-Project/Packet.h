#ifndef PACKET_H
#define PACKET_H
#include "Mesh.h"
#include "Shader.h"


class Packet : public Mesh
{
public:
	Packet();
	Cylinder cylinder;
	Mesh circleTop;
	Mesh circleBottom;

	void Draw(Shader& shader, glm::mat4 proj, glm::mat4 view);

private:

	void genPacket();

};

#endif