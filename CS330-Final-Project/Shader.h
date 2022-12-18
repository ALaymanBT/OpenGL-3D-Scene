#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <GL/glew.h>


class Shader {
public:
	// Shader program ID
	unsigned int ID;

	// Default constructor. Reads filepath to shader source code and compiles shaders
	Shader(const char* vPath, const char* fPath);

	// Use the shader object
	void use();

};
#endif