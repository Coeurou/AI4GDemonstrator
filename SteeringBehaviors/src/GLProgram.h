#pragma once

#include <GL/glew.h>

class GLProgram
{
public:
			GLProgram();
			~GLProgram();
	bool	AddShader(GLenum shaderType, const char* shaderSource);
	bool	LinkProgram();
	void	UseProgram() const											{ glUseProgram(programID); }
	GLuint	GetProgramID() const										{ return programID; }

private:
	GLuint	programID;
};

