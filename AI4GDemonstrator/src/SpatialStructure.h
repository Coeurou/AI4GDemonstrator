#pragma once

#include <glm.hpp>

struct SpatialStructure
{
	glm::vec2 position;
	float orientation;
	glm::vec2 velocity;
	float rotation;

	SpatialStructure() : position(), orientation(0.0f), velocity(), rotation(0.0f) {}
};