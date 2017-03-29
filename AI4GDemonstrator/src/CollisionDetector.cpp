#include "CollisionDetector.h"
#include <limits>

CollisionDetector::CollisionDetector()
{}

CollisionDetector::~CollisionDetector()
{}

glm::vec2 IntersectBox(const glm::vec2& position, const glm::vec2& ray, AABB box)
{
	glm::vec2 invDir = 1.0f / ray;
	auto tMin = (box.min - position) * invDir;
	auto tMax = (box.max - position) * invDir;

	auto t1 = glm::min(tMin, tMax);
	auto t2 = glm::max(tMin, tMax);

	auto tNear = std::fmaxf(t1.x, t1.y);
	auto tFar = std::fminf(t2.x, t2.y);

	auto intersection = glm::vec2(tNear, tFar);
	if (tNear > tFar || tFar < 0) {
		intersection = glm::vec2(std::numeric_limits<float>::max());
	}

	return intersection;
}

Collision CollisionDetector::GetCollision(const std::vector<AABB> obstacles, const glm::vec2& position, const glm::vec2& ray)
{
	Collision collision;
	collision.position = glm::vec2(std::numeric_limits<float>::max());
	AABB collider{ glm::vec2(0), glm::vec2(0) };

	for (auto obstacle : obstacles) {
		auto tempIntersection = IntersectBox(position, ray, obstacle);
		if (glm::length(tempIntersection) < glm::length(collision.position)) {
			collision.position = tempIntersection;
			collider = obstacle;
		}
	}

	collision.normal = glm::vec2(collider.min.y, -collider.min.x);

	return collision;
}
