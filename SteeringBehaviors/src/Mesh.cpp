#include "Mesh.h"
#include <limits>

const GLenum iboTarget = GL_ELEMENT_ARRAY_BUFFER;
const GLenum vboTarget = GL_ARRAY_BUFFER;

Mesh::Mesh() : vao(0), vbo(0), ibo(0)
{}

Mesh::Mesh(const Mesh& m) : vertices(m.vertices), indices(m.indices), textures(m.textures)
{
	InitMesh(vertices, indices, textures);
}

Mesh::Mesh(Mesh&& m) noexcept 
	: vertices(std::move(m.vertices)), indices(std::move(m.indices)), textures(std::move(m.textures))
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
	textures = m.textures;

	InitMesh(vertices, indices, textures);

	return *this;
}

Mesh& Mesh::operator=(Mesh&& m) noexcept
{
	if (this == &m) { return *this; }
	vertices = std::move(m.vertices);
	indices = std::move(m.indices);
	textures = std::move(m.textures);
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
bool Mesh::InitMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

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
==============================================================
void Render const

Render the mesh, bind VAO, textures and buffers before drawing
==============================================================
*/
void Mesh::Render() const
{
	for (size_t i = 0; i < textures.size(); i++)
	{
		textures[i].SetActive(GL_TEXTURE0 + i);
	}

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

glm::vec3 Mesh::GetSize()
{
	if (glm::length(size) < std::numeric_limits<float>::epsilon()) {
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		for (auto& v : vertices) {
			minX = (v.position.x < minX) ? v.position.x : minX;
			minY = (v.position.y < minY) ? v.position.y : minY;
			minZ = (v.position.z < minZ) ? v.position.z : minZ;

			maxX = (v.position.x > maxX) ? v.position.x : maxX;
			maxY = (v.position.y > maxY) ? v.position.y : maxY;
			maxZ = (v.position.z > maxZ) ? v.position.z : maxZ;
		}
		size = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
	}
	return size;
}
