#include "GLProgram.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>

GLProgram::GLProgram()
{
	programID = glCreateProgram();
}


GLProgram::~GLProgram()
{
	glDeleteProgram(programID);
}

/*
=============================================================================================================
void AddShader(GLenum shaderType, const char* shaderSource)

Create a shader (VERTEX, FRAGMENT, GEOMETRY TCS, TES), assign its source, build and verify compilation errors.
Attach it to the program if no errors.
=============================================================================================================
*/
bool GLProgram::AddShader(GLenum shaderType, const char* shaderSource)
{
	GLuint shaderID = glCreateShader(shaderType);

	glShaderSource(shaderID, 1, &shaderSource, nullptr);

	glCompileShader(shaderID);

	GLint compileStatus = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		std::array<GLchar, 512> infoLog;
		glGetShaderInfoLog(shaderID, (GLsizei)infoLog.size(), nullptr, infoLog.data());
		std::cerr << "Failed to compile vertex shader: " << infoLog.data() << std::endl;
		return false;
	}

	glAttachShader(programID, shaderID);

	glDeleteShader(shaderID);
	return true;
}

/*
======================================================================================
void AddShader(GLenum shaderType, const std::string& filePath)

Create a shader from a file, read the file,
assign shader source to an output stringstream, pass it to the other AddShader method.
======================================================================================
*/
bool GLProgram::AddShader(GLenum shaderType, const std::string& filePath)
{
	std::ifstream ifs;
	ifs.open(filePath);
	if (!ifs.is_open()) {
		return false;
	}

	std::ostringstream shaderSource;
	std::copy(std::istreambuf_iterator<char>(ifs),
			  std::istreambuf_iterator<char>(),
			  std::ostreambuf_iterator<char>(shaderSource));

	return AddShader(shaderType, shaderSource.str().c_str());
}

/*
================================================================
void LinkProgram()

Link a program and verify linking errors. Return linking status.
================================================================
*/
bool GLProgram::LinkProgram()
{
	glLinkProgram(programID);

	GLint linkStatus = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		std::array<GLchar, 512> infoLog;
		glGetProgramInfoLog(programID, (GLsizei)infoLog.size(), nullptr, infoLog.data());
		std::cerr << "Error during program linking: " << infoLog.data() << std::endl;
		return false;
	}
	return true;
}

