#pragma once

#include <GL/glew.h>
#include <string>

class GLProgram
{
public:
			GLProgram();
			~GLProgram();
	bool	AddShader(GLenum shaderType, const char* shaderSource);
	bool	AddShader(GLenum shaderType, const std::string& filePath);
	bool	LinkProgram();
	void	UseProgram() const											{ glUseProgram(programID); }
	GLuint	GetProgramID() const										{ return programID; }

private:
	GLuint	programID;
};

