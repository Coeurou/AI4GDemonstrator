#pragma once

#include <glm.hpp>

static glm::vec2 AngleAsVector(float angleInRadians)
{
	return glm::vec2(-sin(angleInRadians), cos(angleInRadians));
}

static float GetNewOrientation(const glm::vec2& velocity, float currentOrientation)
{
	return (glm::length(velocity) > 0) ? atan2(-velocity.x, velocity.y) : currentOrientation;	
}