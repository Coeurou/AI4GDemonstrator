#pragma once

#include <glm.hpp>
#include <limits>
#include <vector>
#include "Segment.h"

/*
===============================================================================

Define a path as a collection of finite segments

===============================================================================
*/
class Path
{

public:
										Path();
										Path(const Path& path);
										Path(Path&& path);
										~Path();

	Path&								operator=(const Path& path);
	Path&								operator=(Path&& path);

	float								GetDistance(const glm::vec2& pos);
	glm::vec2							GetPosition(float distance);

private:
	std::vector<Segment> segments;
};