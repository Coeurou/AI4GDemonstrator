#pragma once
#include <GL/glew.h>
#include "Path.h"
#include "Vertex.h"

class RenderablePath
{
public:
								RenderablePath(const Path& path);
								RenderablePath(Path&& path);
								~RenderablePath();

	glm::vec3					GetPathColor() const				 { return pathColor; }
	void						SetPathColor(const glm::vec3& color) { pathColor = color; }

	void						InitGeometry();
	void						Render();

private:
	Path						path;
	GLuint						vao;
	GLuint						vbo;
	std::vector<ColoredVertex>	vertices;
	glm::vec3					pathColor;

	void						CreateVertexArray();
	void						InitBuffers();
	void						InitVertexArray();
};