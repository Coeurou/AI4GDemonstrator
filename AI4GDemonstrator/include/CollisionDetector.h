#pragma once

#include "Collision.h"
#include "AABB.h"
#include <glm.hpp>
#include <vector>

/*
===============================================================================

Collision detector for bounding box

===============================================================================
*/
class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	Collision GetCollision(const std::vector<AABB> obstacles,
						   const glm::vec2& position, const glm::vec2& ray);
};