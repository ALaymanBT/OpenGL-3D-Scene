#define STB_IMAGE_IMPLEMENTATION
#include "Shader.h"
#include "Mesh.h"
#include "Mug.h"
#include "Biscuit.h"
#include "Packet.h"
#include "Camera.h"
#include "Lights.h"
#include <GLFW/glfw3.h>


bool Initialize(GLFWwindow** window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseInput_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Window size
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Main camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Variables to ensure a constant movement speed
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Variables for cursor offset, initially set to half the screen size
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

// Global bool to prevent camera jump
bool mouseFirst = true;

// Bool to switch between perspective and ortho, defaults to perspective
bool viewType = true;

int main() {

	// Create a window pointer to pass to Initialize()
	GLFWwindow* mainWindow = nullptr;

	// Terminate program if GLFW and GLEW do not initialize properly
	if (!Initialize(&mainWindow)) {
		return -1;
	};


	// Create a new shader from text file source code
	Shader shaderProgram("resources/shaders/vertexShader.txt", "resources/shaders/fragmentShader.txt");

	// Create a obejcts for scene
	Mug mug = Mug();
	Packet packet1 = Packet();
	Packet packet2 = Packet();
	Biscuit biscuit1 = Biscuit();
	Biscuit biscuit2 = Biscuit();
	Mesh plane = Mesh();
	plane.genPlane();

	// Generate textures for the plane
	plane.genTexture("resources/textures/table.jpg");
	plane.genTexture("resources/textures/table.jpg");

	

	// Enable depth buffer and viewport
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WIDTH, HEIGHT);

	// Hide the cursor and set callbacks
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mainWindow, mouseInput_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);
	glfwSetKeyCallback(mainWindow, key_callback);

	// Set uniform locations for textures
	shaderProgram.use();
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture2"), 1);

	// Create light objects and set position/direction, color and intensity
	Lights keyLight = Lights();
	keyLight.setPosition(5.0f, -10.0f, 0.0f);
	keyLight.setColor(0.93f, 0.92f, 0.7f);
	keyLight.setIntensity(1.0f);

	Lights fillLight1 = Lights();
	Lights fillLight2 = Lights();
	Lights fillLight3 = Lights();

	fillLight1.setPosition(10.0f, 0.0f, 0.0f);
	fillLight1.setColor(1.0f, 1.0f, 1.0f);
	fillLight1.setIntensity(0.3f);

	fillLight2.setPosition(-10.0f, 0.0f, 0.0f);
	fillLight2.setColor(1.0f, 1.0f, 1.0f);
	fillLight2.setIntensity(0.3f);

	fillLight3.setPosition(0.0f, 0.0f, 10.0f);
	fillLight3.setColor(1.0f, 1.0f, 1.0f);
	fillLight3.setIntensity(0.3f);

	// Render loop
	while (!glfwWindowShouldClose(mainWindow)) {

		float currentFrame = glfwGetTime();

		// Set the time difference between current and last frame
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(mainWindow);

		// Matrices for view and projection transformations
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		// Set background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set view and projection matrices
		view = camera.getViewMatrix();

		shaderProgram.use();

		// Set the view position for specular calculation
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "viewPos"), 1, glm::value_ptr(camera.position));

		// Specularity values of key and fill lights
		glm::vec3 specularKey = glm::vec3(0.3f);
		glm::vec3 specularFill = glm::vec3(3.0f);

		// Set light direction, color, and intensity of key and fill lights
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[0].diffuse"), 1, glm::value_ptr(keyLight.getColor()));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[0].specular"), 1, glm::value_ptr(specularKey));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[0].direction"), 1, glm::value_ptr(keyLight.getPosition()));
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "dirLight[0].intensity"), keyLight.getIntensity());

		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[1].diffuse"), 1, glm::value_ptr(fillLight1.getColor()));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[1].direction"), 1, glm::value_ptr(fillLight1.getPosition()));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[1].specular"), 1, glm::value_ptr(specularFill));
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "dirLight[1].intensity"), fillLight1.getIntensity());

		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[2].diffuse"), 1, glm::value_ptr(fillLight2.getColor()));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[2].direction"), 1, glm::value_ptr(fillLight2.getPosition()));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[2].specular"), 1, glm::value_ptr(specularFill));
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "dirLight[2].intensity"), fillLight2.getIntensity());

		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[3].diffuse"), 1, glm::value_ptr(fillLight3.getColor()));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[3].direction"), 1, glm::value_ptr(fillLight3.getPosition()));
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "dirLight[3].specular"), 1, glm::value_ptr(specularFill));
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "dirLight[3].intensity"), fillLight3.getIntensity());

		// Swap between perspective and orthographic according to view type boolean
		if (viewType == true) {
			projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		}
		else {
			projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
		}

		// Transform meshes to the proper positions
		plane.scaleMesh(4.0f, 1.0f, 4.0f);
		mug.translateMesh(1.0f, 1.0f, 0.0f);
		biscuit1.translateMesh(-1.5f, 0.055f, 2.0f);
		biscuit1.scaleMesh(1.7f, 1.0f, 1.0f);
		biscuit2.translateMesh(-1.5f, 0.15f, 1.95f);
		biscuit2.rotateMesh(glm::radians(353.0f),1.0f, 0.0f, 0.0f);
		biscuit2.scaleMesh(1.0f, 1.0f, 1.5f);
		packet1.translateMesh(-2.0f, 0.04f, -1.0f);
		packet1.scaleMesh(1.0f, 0.05f, 1.0f);
		packet1.rotateMesh(glm::radians(60.0f), 0.0f, 1.0f, 0.0f);
		packet2.translateMesh(-1.5f, 0.04f, -0.3f);
		packet2.scaleMesh(1.0f, 0.05f, 1.0f);
		packet2.rotateMesh(glm::radians(90.0f), 0.0f, 1.0f, 0.0f);

		// Draw the meshes
		plane.Draw(1.0, shaderProgram, projection, view);
		mug.Draw(shaderProgram, projection, view);
		biscuit1.Draw(shaderProgram, projection, view);
		biscuit2.Draw(shaderProgram, projection, view);
		packet1.Draw(shaderProgram, projection, view);
		packet2.Draw(shaderProgram, projection, view);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}


	return 0;

}


bool Initialize(GLFWwindow** window)
{
	/*
* Initialize GLFW and specify minimum version of OpenGL that to be used, 3(major).3(minor)
* then specify that the core-profile should be used
*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable forward compatability bit for MacOS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a new GLFW window
	* window = glfwCreateWindow(WIDTH, HEIGHT, "Final Project_ALayman", NULL, NULL);
	if (*window == NULL) {
		std::cout << "Window failed to open.\n";
		glfwTerminate();
		return false;
	}

	// Set the frame buffer size callback function
	glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

	// Set current context to created window
	glfwMakeContextCurrent(*window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW Failed to initialize\n";
		return false;
	}

	return true;
}

// Callback to resize viewport when user resizes window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	// Pass Y offset to camera object
	camera.scrollInput(yoffset);
}

void mouseInput_callback(GLFWwindow* window, double xPos, double yPos) {

	// If this is the first mouse input, set the last positions to current to prevent camera jump
	if (mouseFirst == true) {
		lastX = xPos;
		lastY = yPos;
		mouseFirst = false;
	}

	// Set the offset to the current cursor position minus the previous position
	float offsetX = xPos - lastX;
	float offsetY = lastY - yPos;

	// Set the previous cursor position to the current position
	lastX = xPos;
	lastY = yPos;

	// Pass X and Y offsets to camera object
	camera.mouseInput(offsetX, offsetY);
}

// Process user input
void processInput(GLFWwindow* window) {

	// Close the window if esc is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Move forwared or backward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.keyboardMovement(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.keyboardMovement(BACKWARD, deltaTime);
	}

	// Move left or right
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.keyboardMovement(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.keyboardMovement(RIGHT, deltaTime);
	}

	// Move up or down
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.keyboardMovement(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.keyboardMovement(UP, deltaTime);
	}

}

// Callback to check for a single key press
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		// Swap from orthographic to perspective
		if (viewType == false) {
			viewType = true;
		}

		// Swap from perspective to orthographic
		else {
			viewType = false;
		}
	}
}