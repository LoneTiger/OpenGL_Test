#ifndef SHADER_H
#define SHADER_H

#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
	// Program ID
	unsigned int ID;

	// Constructor
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use/activate the shaderset
	void use();

	// Uniform functions
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
};

#endif