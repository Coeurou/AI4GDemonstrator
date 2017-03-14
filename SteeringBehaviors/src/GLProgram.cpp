#include "GLProgram.h"
#include <array>
#include <iostream>

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

