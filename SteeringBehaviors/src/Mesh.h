#pragma once

#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class Mesh
{
public:
						Mesh();
						Mesh(const Mesh& m);
						Mesh(Mesh&& m) noexcept;
						~Mesh();

	Mesh&				operator=(const Mesh& m);
	Mesh&				operator=(Mesh&& m) noexcept;

	bool				InitMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	void				Render() const;

private:
	GLuint				vao;
	GLuint				vbo;
	GLuint				ibo;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};