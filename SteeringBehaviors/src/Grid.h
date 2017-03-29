#pragma once

#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class Grid
{
public:
								Grid(GLushort width, GLushort depth);
								~Grid();

	GLushort					GetWidth() const		{ return width; }
	void						SetWidth(GLushort w)	{ width = w; }
	GLushort					GetDepth() const		{ return depth; }
	void						SetDepth(GLushort d)	{ depth = d; }

	bool						InitGeometry();
	void						Render() const;

private:
	GLushort					width;
	GLushort					depth;
	GLushort					halfWidth;
	GLushort					halfDepth;

	GLuint						vao;
	GLuint						vbo;
	GLuint						ibo;

	std::vector<ColoredVertex>	vertices;
	std::vector<GLushort>		indices;

	void						CreateVertexArray();
	void						InitBuffers();
	void						InitVertexArray();
};