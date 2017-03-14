#pragma once

#include <array>
#include <glm.hpp>

/*
===============================================================================

Define a segment as two points in 2D space

===============================================================================
*/
struct Segment
{
public:
	static const int					NB_POINTS = 2;
	static const float					DISTANCE_NOT_FOUND;
	static const glm::vec2				OUTSIDE_SEGMENT;

	std::array<glm::vec2, NB_POINTS>	points = { glm::vec2(0), glm::vec2(0) };

	glm::vec2							GetSegment() const;
	float								GetLength() const;

	glm::vec2							GetNeareastPoint(const glm::vec2& pos) const;
	glm::vec2							GetPosition(float distance) const;
	float								GetDistance(const glm::vec2& pos) const;
};