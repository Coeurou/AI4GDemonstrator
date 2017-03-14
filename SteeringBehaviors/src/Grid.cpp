#include "Grid.h"

const GLenum iboTarget = GL_ELEMENT_ARRAY_BUFFER;
const GLenum vboTarget = GL_ARRAY_BUFFER;

Grid::Grid(GLushort width, GLushort depth)
	: width(width), depth(depth), halfWidth((GLushort)(width*0.5f)), halfDepth((GLushort)(depth*0.5f)), vao(0), vbo(0), ibo(0)
{
}

Grid::~Grid()
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool Grid::InitGeometry()
{
	// Build rows
	for (GLshort z = -halfDepth; z < halfDepth; z++) {
		for (GLshort x = -halfWidth; x < halfWidth; x += width - 1) {
			vertices.emplace_back(ColoredVertex{ glm::vec3(x, 0, z), glm::vec3(0.5f) });
			indices.emplace_back(vertices.size() - 1);
		}
	}
	// Build columns
	for (GLshort x = -halfWidth; x < halfWidth; x++) {
		for (GLshort z = -halfDepth; z < halfDepth; z += depth - 1) {
			vertices.emplace_back(ColoredVertex{ glm::vec3(x, 0, z), glm::vec3(0.5f) });
			indices.emplace_back(vertices.size() - 1);
		}
	}

	CreateVertexArray();
	InitBuffers();
	InitVertexArray();

	glBindVertexArray(0);
	glBindBuffer(vboTarget, 0);
	glBindBuffer(iboTarget, 0);

	return (indices.size() == vertices.size()-1);
}

void Grid::Render() const
{
	glBindVertexArray(vao);

	glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_SHORT, nullptr);
	
	glBindVertexArray(0);
}

void Grid::CreateVertexArray()
{
	glGenVertexArrays(1, &vao);
}

/*
==================================================================================
void InitBuffers

Create a VBO and an IBO, fill the vbo with vertices data and IBO with indices data
==================================================================================
*/
void Grid::InitBuffers()
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(iboTarget, ibo);
	glBufferData(iboTarget, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(vboTarget, vbo);
	glBufferData(vboTarget, sizeof(ColoredVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
}

/*
======================================================
void InitBuffers

Create a VAO and a VBO, fill the vbo with vertices data
=======================================================
*/
void Grid::InitVertexArray()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, color));
}
