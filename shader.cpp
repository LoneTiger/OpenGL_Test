#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	// Get shader source code
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	// Make sure exception throwing works
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	
	// Open and read vertex shader
	try {
		// Open files
		vShaderFile.open(vertexPath);
		stringstream vShaderStream;

		// Read files
		vShaderStream << vShaderFile.rdbuf();

		// Close files
		vShaderFile.close();

		// Convert to string
		vertexCode = vShaderStream.str();
	}
	// If file open failed
	catch (ifstream::failure e) {
		cout << "Error: Failed to open" << vertexPath << endl;
	}

	// Open and read fragment shader
	try {
		// Open file
		fShaderFile.open(fragmentPath);
		stringstream fShaderStream;

		// Read file
		fShaderStream << fShaderFile.rdbuf();

		// Close file
		fShaderFile.close();

		// Convert to string
		fragmentCode = fShaderStream.str();
	}
	// If file open failed
	catch (ifstream::failure e) {
		cout << "Error: Failed to open" << fragmentPath << endl;
	}

	// Put into string thingys
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile shaders
	unsigned int vertex, fragment; // Shader IDs
	int success;
	char infoLog[512];

	// Import and compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	// If compilation failed
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Error compiling vertex shader: " << infoLog << endl;
	}

	// Import and compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	// If compilation failed
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Error compiling fragment shader: " << infoLog << endl;
	}

	// Shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	// If linking failed
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "Error linking shaders: " << infoLog << endl;
	}

	// Delete shaders now that the program is built
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string& name, float value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
