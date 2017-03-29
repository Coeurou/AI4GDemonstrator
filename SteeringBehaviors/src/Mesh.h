#pragma once

#include <GL/glew.h>
#include <vector>
#include "Vertex.h"
#include "Texture.h"

class Mesh
{
public:
							Mesh();
							Mesh(const Mesh& m);
							Mesh(Mesh&& m) noexcept;
							~Mesh();

	Mesh&					operator=(const Mesh& m);
	Mesh&					operator=(Mesh&& m) noexcept;

	bool					InitMesh(const std::vector<Vertex>& vertices,
									 const std::vector<GLuint>& indices,
									 const std::vector<Texture>& textures);
	void					Render() const;
	glm::vec3				GetSize();

private:
	GLuint					vao;
	GLuint					vbo;
	GLuint					ibo;

	glm::vec3				size;

	std::vector<Vertex>		vertices;
	std::vector<GLuint>		indices;
	std::vector<Texture>	textures;
};