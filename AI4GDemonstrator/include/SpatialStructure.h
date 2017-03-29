#pragma once

#include <glm.hpp>

struct SpatialStructure
{
	glm::vec2 position = glm::vec2(0);
	float orientation = 0.0f;
	glm::vec2 velocity = glm::vec2(0);
	float rotation = 0.0f;
	glm::vec2 size = glm::vec2(1);
};