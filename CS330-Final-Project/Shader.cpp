#include "SHADER.H"

Shader::Shader(const char* vPath, const char* fPath) {
	// Variables to store vertex and fragment source code
	std::string vertexSourceCode;
	std::string fragmentSourceCode;

	// Vertex and fragment input file streams
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	// ifstream can throw exceptions
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open source code files
		vertexFile.open(vPath);
		fragmentFile.open(fPath);


		// Stringstreams to read file contents
		std::stringstream fStream, vStream;
		vStream << vertexFile.rdbuf();
		fStream << fragmentFile.rdbuf();

		// Close files once contents have been read
		vertexFile.close();
		fragmentFile.close();

		// Copy stream contents to source code variables
		vertexSourceCode = vStream.str();
		fragmentSourceCode = fStream.str();

	}

	catch (std::ifstream::failure e) {
		std::cout << "Error reading shader source file.\n";
	}

	// Convert shader source code to C_string
	const char* vCode = vertexSourceCode.c_str();
	const char* fCode = fragmentSourceCode.c_str();

	// Variables for error handling
	int success;
	char infoLog[512];

	// Shader objects
	unsigned int vertex, fragment;

	// Compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex);

	// Confirm vertex shader compiled properly
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Vertex shader failed to compile: " << infoLog << std::endl;
	}

	//Compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment);

	// Confirm fragment shader compiled properly
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Fragment shader failed to compile: " << infoLog << std::endl;
	}

	// Create shader program and link shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// Confirm shaders have linked properly
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Shaders failed to link to program: " << infoLog << std::endl;
	}

	// Delete shaders after linking
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}