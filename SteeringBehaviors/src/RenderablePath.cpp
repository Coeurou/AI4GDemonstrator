#include "RenderablePath.h"
#include "Vertex.h"
#include <vector>

const GLenum vboTarget = GL_ARRAY_BUFFER;
const float pathBreadth = 2.5f;

RenderablePath::RenderablePath(const Path& path) : path(path), vao(0), vbo(0)
{}

RenderablePath::RenderablePath(Path&& path) : path(path), vao(0), vbo(0)
{}

RenderablePath::~RenderablePath()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void RenderablePath::CreateVertexArray()
{
	glGenVertexArrays(1, &vao);
}

/*
=============================================
void InitBuffers

Create a VBO, fill the vbo with vertices data
=============================================
*/
void RenderablePath::InitBuffers()
{	
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(vboTarget, vbo);
	glBufferData(vboTarget, sizeof(ColoredVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
}

/*
===========================================
void InitVertexArray

Configure VAO attributes position and color
===========================================
*/
void RenderablePath::InitVertexArray()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, color));
}

/*
============================================================================================
void InitGeometry

Fill vertices data. Based upon path object which is a vector of segments (2 points in space)
============================================================================================
*/
void RenderablePath::InitGeometry()
{
	auto segments = path.GetSegments();
	vertices.reserve(segments.size() * 2);

	for (auto s : segments) {
		vertices.emplace_back(ColoredVertex{ glm::vec3(s.points[0].x, 0, s.points[0].y), pathColor });
		vertices.emplace_back(ColoredVertex{ glm::vec3(s.points[1].x, 0, s.points[1].y), pathColor });
	}

	CreateVertexArray();
	InitBuffers();
	InitVertexArray();
	glBindBuffer(vboTarget, 0);
	glBindVertexArray(0);
}

/*
=========================================================================================
void Render

Render a path with OpenGL line mode, change point size in order to make the path visible.
=========================================================================================
*/
void RenderablePath::Render()
{
	glBindVertexArray(vao);

	auto previousLineSize = 0.0f;
	glGetFloatv(GL_LINE_WIDTH, &previousLineSize);
	glLineWidth(pathBreadth);

	glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size());

	glLineWidth(previousLineSize);
}
