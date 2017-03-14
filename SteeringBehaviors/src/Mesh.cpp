#include "Mesh.h"

const GLenum iboTarget = GL_ELEMENT_ARRAY_BUFFER;
const GLenum vboTarget = GL_ARRAY_BUFFER;

Mesh::Mesh() : vao(0), vbo(0), ibo(0)
{
}

Mesh::Mesh(const Mesh& m) : vertices(m.vertices), indices(m.indices)
{
	InitMesh(vertices, indices);
}

Mesh::Mesh(Mesh&& m) noexcept : vertices(std::move(m.vertices)), indices(std::move(m.indices))
{	
	m.vao = 0;
	m.vbo = 0;
	m.ibo = 0;
}

Mesh& Mesh::operator=(const Mesh& m)
{
	if (this == &m) { return *this; }
	vertices = m.vertices;
	indices = m.indices;

	InitMesh(vertices, indices);

	return *this;
}

Mesh& Mesh::operator=(Mesh&& m) noexcept
{
	vertices = std::move(m.vertices);
	indices = std::move(m.indices);
	m.vao = 0;
	m.vbo = 0;
	m.ibo = 0;

	return *this;
}

Mesh::~Mesh()
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}

/*
=====================================================================================
void InitMesh

Create VAO, VBO and an IBO, fill the vbo with vertices data and IBO with indices data
=====================================================================================
*/
bool Mesh::InitMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	
	glBindBuffer(iboTarget, ibo);
	glBufferData(iboTarget, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(vboTarget, vbo);
	glBufferData(vboTarget, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	glBindVertexArray(0);
	glBindBuffer(vboTarget, 0);
	glBindBuffer(iboTarget, 0);

	return (vertices.size() > 0 && indices.size() > 0);
}

/*
=================================================
void Render const

Render the mesh, bind VAO and buffers before draw
=================================================
*/
void Mesh::Render() const
{
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}