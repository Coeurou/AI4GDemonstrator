#pragma once

#include <glm.hpp>

struct ColoredVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};