#ifndef MESH_H
#define MESH_H
#include <string>
#include "stb_image.h"
#include <vector>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>





class Mesh {
public:

	// Default constructor
	Mesh();

	// Constructor
	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);

	// Drawing requires a shader object reference to be passed in
	void Draw(float pctOfShapeToDraw, Shader& shader, glm::mat4 proj, glm::mat4 view);
	void genTexture(const char* texPath);

	// Functions to set and get vertices and indices
	void setVertices(std::vector<float> vertices);
	std::vector<float> getVertices();
	void setIndices(std::vector<unsigned int> indices);
	std::vector<unsigned int> getIndices();
	void addVertex(float vertex);

	// Primitave shape functions
	void genCircle(unsigned int numSectors, float radius, float height);
	float genRandomColorVertex();
	void genPlane();

	// Translate across axes
	void translateMesh(float x, float y, float z);
	void scaleMesh(float x, float y, float z);
	void rotateMesh(float angle, float x, float y, float z);
	void updateTransformationMatrix(glm::mat4 transformation);


protected:

	// Variables for transformation
	glm::mat4 trans = glm::mat4(1.0f);

	// Mesh data
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// Vertex array object, vertex buffer object, and element buffer objects to render data
	unsigned int vao, vbo, ebo, textureCount, texture1, texture2, texture3, texture4;


	// Generates and binds buffer objects to a vao and enables attribute pointers
	void setupMesh();
	void calculateNormals();

private:


};

// Subclass for cylinders
class Cylinder : public Mesh {

public:
	Cylinder();
	// Constructor
	Cylinder(unsigned int numSectors, float height, float radius);

private:
	void genCylinderIndices(unsigned int numSectors);
	void genCircleVertices(unsigned int numSectors, float radius, float height);

};

//Subclass for tori
class Torus : public Mesh {

public:
	Torus();
	// Constructor
	Torus(int numMainSegments, int numTubeSegments, float mRadius, float tRadius);

private:
	void genTorusVertices(int numMainSegments, int numTubeSegments, float mRadius, float tRadius);
	void genTorusIndices(int numMainSegments, int numTubeSegments);

};

#endif
