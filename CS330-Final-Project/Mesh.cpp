#include "Mesh.h"

// Value of Pi
const double PI = 3.141592653589;

// Default constructor is used when primitive shapes must be generated
Mesh::Mesh() {

	// Initialize array and buffer objects to null
	vao = NULL;
	vbo = NULL;
	ebo = NULL;
	texture1 = NULL;
	texture2 = NULL;
	textureCount = 0;
}

// This constructor is called when vertices and indices do not need to be generated
Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	// Set member data to input data
	setVertices(vertices);
	setIndices(indices);
	textureCount = 0;
	texture1 = NULL;
	texture2 = NULL;

	// Build the mesh
	setupMesh();
}

void Mesh::setupMesh()
{

	// Create vertex array, vertex buffer, and element buffer objects
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Bind the vao
	glBindVertexArray(vao);

	// Bind vbo and define vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// Bind ebo and define index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind vao
	glBindVertexArray(0);
}

void Mesh::genTexture(const char* texPath) {
	// Push back a null object for next texture data
	glBindVertexArray(vao);

	// Create texture data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texPath, &width, &height, &nrChannels, 0);


	// Generate and bind texture object(s)
	stbi_set_flip_vertically_on_load(true);

	switch (textureCount) {
	case 0:
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// Attach image to currently bound texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		break;

	case 1:
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// Attach image to currently bound texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		break;

	case 2:
		glGenTextures(1, &texture3);
		glBindTexture(GL_TEXTURE_2D, texture3);
		// Attach image to currently bound texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		break;

	case 3:
		glGenTextures(1, &texture4);
		glBindTexture(GL_TEXTURE_2D, texture4);
		// Attach image to currently bound texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		break;

	default:
		break;

	}



	stbi_image_free(data);

	// Increase number of textures on mesh
	textureCount++;
	glBindVertexArray(0);
}


void Mesh::updateTransformationMatrix(glm::mat4 transformation)
{
	// Multiply the passed in transformation with the current transformation matrix
	trans = transformation * trans;
}

void Mesh::Draw(float pctOfShapeToDraw, Shader& shader, glm::mat4 proj, glm::mat4 view)
{

	// Use the referenced shader program
	shader.use();

	// Set the diffuse and specular maps as well as the shininess for the material
	glUniform1i(glGetUniformLocation(shader.ID, "material.diffuse1"), 0);
	glUniform1i(glGetUniformLocation(shader.ID, "material.specular1"), 1);
	glUniform1i(glGetUniformLocation(shader.ID, "material.diffuse2"), 2);
	glUniform1i(glGetUniformLocation(shader.ID, "material.specular2"), 3);
	glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 32.0f);

	// The percent as a decimal of the shape to draw
	float shapePercent = pctOfShapeToDraw;

	// Sets fragment shader bool to true if a mesh has multiple textures
	if (textureCount > 2) {
		glUniform1i(glGetUniformLocation(shader.ID, "multipleTextures"), true);
	}
	else {
		glUniform1i(glGetUniformLocation(shader.ID, "multipleTextures"), false);
	}


	// Find shader transform uniform and pass in the trans matrix
	unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->trans));
	unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


	// Bind the active textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture4);

	// Bind the vao and texture, then draw the mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size() * shapePercent, GL_UNSIGNED_INT, 0);

	// Unbind vao
	glBindVertexArray(0);

	// Set transformation matrix back to default
	this->trans = glm::mat4(1.0f);
}

void Mesh::translateMesh(float x, float y, float z)
{
	// Set vec3 with passed in values and multiply trans matrix
	glm::vec3 translation = glm::vec3(x, y, z);
	trans = glm::translate(trans, translation);

}

void Mesh::scaleMesh(float x, float y, float z)
{
	// Set vec3 with passed in values and multiply trans matrix
	glm::vec3 scale = glm::vec3(x, y, z);
	trans = glm::scale(trans, scale);

}

void Mesh::rotateMesh(float angle, float x, float y, float z)
{
	// Set vec3 with passed in values and multiply trans matrix. Angle is the angle of rotation in radians, use time for continuous rotation
	glm::vec3 rotate = glm::vec3(x, y, z);
	this->trans = glm::rotate(this->trans, angle, rotate);

}

float Mesh::genRandomColorVertex()
{
	// Generate a random number and divide by the maximum random number to obtain a float between 0.0 and 1.0
	float r = rand() / (float)RAND_MAX;
	return r;
}

// Set the vertices
void Mesh::setVertices(std::vector<float> vertices)
{
	this->vertices = vertices;
}

void Mesh::addVertex(float vertex) {
	this->vertices.push_back(vertex);
}

// Retrieve vertices
std::vector<float> Mesh::getVertices()
{
	return this->vertices;
}

// Set the indices
void Mesh::setIndices(std::vector<unsigned int> indices)
{
	this->indices = indices;
}
// Retrieve the indices
std::vector<unsigned int> Mesh::getIndices()
{
	return this->indices;
}

// For a separate circle mesh
void Mesh::genCircle(unsigned int numSectors, float height, float radius) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int numSides = numSectors;
	float r = radius;



	// Center point
	addVertex(0.0);	addVertex(height);		addVertex(0.0);

	// First normal
	addVertex(0.0);	addVertex(0.0);	addVertex(0.0);

	// First Texture

	addVertex(0.5f);	addVertex(0.5f);



	for (int i = 0; i < numSides; i++) {

		// Angle of theta for current sector of the circle
		float theta = (i * 2 * PI) / numSides;
		float texCoord = (float)i / numSides;

		// Side points
		addVertex(r * cos(theta));
		addVertex(height);
		addVertex(r * sin(theta));

		// Color
		addVertex(0.0f);
		addVertex(0.0f);
		addVertex(0.0f);

		// Texture Coords
		addVertex(((r * cos(theta)) / r + 1) * 0.5);
		addVertex(((r * sin(theta)) / r + 1) * 0.5);
	}

	int center = 0;
	int vertexOffset = 1;

	// Gen indices for circle
	for (int i = 1; i < numSides; i++) {

		indices.push_back(center);
		indices.push_back(vertexOffset);
		indices.push_back(vertexOffset + 1);

		// Increase offset
		vertexOffset++;
	}
	indices.push_back(center);
	indices.push_back(vertexOffset);
	indices.push_back(1);

	setIndices(indices);
	calculateNormals();
	setupMesh();
}

// Generate circles for a cylinder
void Cylinder::genCircleVertices(unsigned int numSectors, float radius, float height) {

	std::vector<float> vertices;
	unsigned int numSides = numSectors;
	float r = radius;

	// Center point
	addVertex(0.0);	addVertex(height);		addVertex(0.0);

	// First Normal
	addVertex(0.0);	addVertex(0.0);	addVertex(0.0);

	// First Texture for top or bottom of cylinder
	if (height < 0.0) {
		addVertex(0.0f);	addVertex(0.0f);
	}
	else {
		addVertex(0.0f);	addVertex(1.0f);
	}


	for (int i = 0; i < numSides; i++) {

		// Angle of theta for current sector of the circle
		float theta = (i * 2 * PI) / numSides;
		float texCoord = (float)i / numSides;

		// Side points
		addVertex(r * cos(theta));
		addVertex(height);
		addVertex(r * sin(theta));

		// Normals set to 0
		addVertex(0.0f);
		addVertex(0.0f);
		addVertex(0.0f);

		// Texture Coords for bottom of cylinder
		if (height < 0.0) {

			addVertex(texCoord);
			addVertex(0.0f);
		}
		// Texture Coords for top of cylinder
		else {
			addVertex(texCoord);
			addVertex(1.0f);
		}


	}

}

void Mesh::genPlane() {
	std::vector<float> vertices = {
		//Vertex				//Normals			//TexCoords
		-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,	0.0f, 1.0f, //Back left
		1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,	1.0f, 1.0f, //Back right
		1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, //Front right
		-1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f  //Front left
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		0, 2, 3
	};

	setVertices(vertices);
	setIndices(indices);
	calculateNormals();
	setupMesh();
}

Torus::Torus() {
	this->trans = trans;
}

/*Constructor for torus object
*
* main segments is the number of circles within the tube to generate
* tube segments is the number of sides of each circle in the tube
* mRadius is the main radius and tRadius is the tube radius
*/
Torus::Torus(int numMainSegments, int numTubeSegments, float mRadius, float tRadius)
{
	this->trans = trans;

	// Pass segment and radius input to generate vertices
	genTorusVertices(numMainSegments, numTubeSegments, mRadius, tRadius);

	// Once vertices are generated, generate indices to draw using a triangle strip
	genTorusIndices(numMainSegments, numTubeSegments);

	calculateNormals();
	// Build the torus
	setupMesh();
}

// Generate the torus vertices
void Torus::genTorusVertices(int numMainSegments, int numTubeSegments, float mRadius, float tRadius)
{
	// Vector to store vertex data
	std::vector<float> vertices;

	float mainRadius = mRadius;
	float tubeRadius = tRadius;
	int mainSegments = numMainSegments;
	int tubeSegments = numTubeSegments;

	// Value to increase every iteration in radians
	float mainSegmentAngleStep = glm::radians(360.0f / float(mainSegments));
	float tubeSegmentAngleStep = glm::radians(360.0f / float(tubeSegments));

	// Set initial main segment angle to 0
	float currentMainSegmentAngle = 0.0f;

	//Texture Variables
	float mainSegmentTextureStep = 2.0f / float(mainSegments);
	float tubeSegmentTextureStep = 1.0f / float(tubeSegments);
	float currMainTexCoord = 0.0f;

	// Main loop, loops number of main segments + 1 to generate the final segment
	for (int i = 0; i <= mainSegments; i++) {

		float currTubeTexCoord = 0.0f;

		// sine and cosine of the current angle of the main segment
		float sinMainSegment = sin(currentMainSegmentAngle);
		float cosMainSegment = cos(currentMainSegmentAngle);

		// Set initial tube segment angle to 0
		float currentTubeSegmentAngle = 0.0f;

		// Loop for each circle that comprises the tube
		for (int j = 0; j <= tubeSegments; j++) {

			// sine and cosine of current angle of the tube segment
			float sinTubeSegment = sin(currentTubeSegmentAngle);
			float cosTubeSegment = cos(currentTubeSegmentAngle);

			// Add vertex to vertices vector
			addVertex((mainRadius + tubeRadius * cosTubeSegment) * cosMainSegment);	// x pos
			addVertex((mainRadius + tubeRadius * cosTubeSegment) * sinMainSegment);	// y pos
			addVertex((tubeRadius * sinTubeSegment));									// z pos


			// Colors required due to mesh setup stride
			addVertex(0.0f);
			addVertex(0.0f);
			addVertex(0.0f);

			// TexCoords
			addVertex(currTubeTexCoord);
			addVertex(currMainTexCoord);


			// Increase tube segment angle by angle step value
			currentTubeSegmentAngle += tubeSegmentAngleStep;

			currTubeTexCoord += tubeSegmentTextureStep;
		}

		// Increase main segment angle by angle step value
		currentMainSegmentAngle += mainSegmentAngleStep;

		currMainTexCoord += mainSegmentTextureStep;
	}

	// Set the torus vertices
	//setVertices(vertices);
}

void Torus::genTorusIndices(int numMainSegments, int numTubeSegments)
{
	// Vector to store indices
	std::vector<unsigned int> indices;

	unsigned int vertexOffset = 0;

	// Loop through the number of main segments
	for (int i = 0; i < numMainSegments; i++) {

		// Generate vertices for every tube segment for each main segment
		for (int j = 0; j <= numTubeSegments; j++) {

			// Set vertexA to the current offset and vertexB to the same vertex of the next circle in the tube
			unsigned int segmentAStart = vertexOffset;
			unsigned int segmentBStart = vertexOffset + numTubeSegments;

			// Add vertices to index vector
			indices.push_back(segmentAStart);
			indices.push_back(segmentAStart + 1);
			indices.push_back(segmentBStart);

			indices.push_back(segmentBStart);
			indices.push_back(segmentAStart + 1);
			indices.push_back(segmentBStart + 1);

			// Increase the vertex offset
			vertexOffset++;
		}

	}

	// Set the indices
	setIndices(indices);
}

Cylinder::Cylinder() {

}

Cylinder::Cylinder(unsigned int numSectors, float height, float radius)
{

	// Generate two circles and assign the y value half the desired height
	genCircleVertices(numSectors, radius, height / 2);
	genCircleVertices(numSectors, radius, -height / 2);

	// Generate the indices, calculate weighted normals, and create the mesh
	genCylinderIndices(numSectors);
	calculateNormals();
	setupMesh();
}

void Cylinder::genCylinderIndices(unsigned int numSectors)
{
	std::vector<unsigned int> indices;
	int vertexOffset;

	// Variables for connecting top and bottom circles
	int c1;
	int c2;

	// Center points of both circles
	int topCenter = 1;
	int bottomCenter = numSectors + 2;

	// Set initial values to circle centers
	c1 = topCenter;
	c2 = bottomCenter;

	// Loop through each sector of top and bottom circles and connect them
	for (int i = 0; i < numSectors - 1; i++, c1++, c2++) {



		// Connects two vertices of top circle to one of the bottom circle
		indices.push_back(c1);
		indices.push_back(c1 + 1);
		indices.push_back(c2);

		// Connects two vertices of bottom circle to one of the top circle
		indices.push_back(c2);
		indices.push_back(c1 + 1);
		indices.push_back(c2 + 1);


	}

	// Add the final indices to attach beginning and end of the cylinder
	indices.push_back(1);
	indices.push_back(numSectors);
	indices.push_back(numSectors + 2);


	indices.push_back(numSectors + 2);
	indices.push_back(numSectors * 2 + 1);
	indices.push_back(numSectors);

	setIndices(indices);
}

void Mesh::calculateNormals() {

	// Calculate pyramid normals and replace zeros in vertex array
	for (int i = 0; i < indices.size(); i += 3) {

		//std::vector<float> vertices = getVertices();
		//std::vector<unsigned int> indices = getIndices();

		// Variables for start of next vertex in array
		int vertexA = indices.at(i) * 8;
		int vertexB = indices.at(i + 1) * 8;
		int vertexC = indices.at(i + 2) * 8;

		// Create a vec3 for each vertex
		glm::vec3 A = glm::vec3(vertices.at(vertexA), vertices.at(vertexA + 1), vertices.at(vertexA + 2));
		glm::vec3 B = glm::vec3(vertices.at(vertexB), vertices.at(vertexB + 1), vertices.at(vertexB + 2));
		glm::vec3 C = glm::vec3(vertices.at(vertexC), vertices.at(vertexC + 1), vertices.at(vertexC + 2));

		// Find the distance from B to A and C to A
		glm::vec3 edgeAB = B - A;
		glm::vec3 edgeAC = C - A;

		// Get the cross product of the edges to find the normal of a face (Clockwise winding)
		glm::vec3 weightedNormal = glm::cross(edgeAC, edgeAB);

		// Add the weighted normal to each vertex to create a smooth transition between faces
		vertices.at(vertexA + 3) += weightedNormal.x;
		vertices.at(vertexA + 4) += weightedNormal.y;
		vertices.at(vertexA + 5) += weightedNormal.z;

		vertices.at(vertexB + 3) += weightedNormal.x;
		vertices.at(vertexB + 4) += weightedNormal.y;
		vertices.at(vertexB + 5) += weightedNormal.z;

		vertices.at(vertexC + 3) += weightedNormal.x;
		vertices.at(vertexC + 4) += weightedNormal.y;
		vertices.at(vertexC + 5) += weightedNormal.z;

	}

	// Start at the first normal coordinate and increment by 8 (stride)
	for (int i = 3; i < vertices.size(); i += 8) {

		// Normalize each vertex normal vector
		glm::vec3 normal = glm::vec3(vertices.at(i), vertices.at(i + 1), vertices.at(i + 2));
		glm::vec3 norm = glm::normalize(normal);

		// Set the normals in the array to the normalized coordinates
		this->vertices.at(i) = norm.x;
		this->vertices.at(i + 1) = norm.y;
		this->vertices.at(i + 2) = norm.z;

	}

}
