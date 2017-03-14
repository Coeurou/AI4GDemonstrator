#include "Path.h"
#include <iostream>

Path::Path() 
{}

Path::Path(const Path& path) : segments(path.segments)
{}

Path::Path(Path&& path) : segments(std::move(path.segments))
{}

Path::~Path() 
{}

Path& Path::operator=(const Path& path) 
{
	segments = path.segments; return *this; 
}

Path& Path::operator=(Path&& path) 
{ 
	segments = std::move(path.segments); return *this;
}

/*
===============================================================================================
GetDistance

Return the distance between path first point and pos parameter, this works
only if pos lay on the path. (see also Segment::GetDistance)
===============================================================================================
*/
float Path::GetDistance(const glm::vec2& pos)
{
	float distance = 0.0f;
	float cumulativeDistance = 0.0f;

	for (auto segment : segments) {
		distance = segment.GetDistance(pos);

		if (distance != Segment::DISTANCE_NOT_FOUND) {
			return cumulativeDistance + distance;
		}
		cumulativeDistance += segment.GetLength();
	}

	std::cerr << "[Segment::GetDistance] Passed parameter position must laid on path" << std::endl;

	return Segment::DISTANCE_NOT_FOUND;
}

/*
===============================================================================================
GetPosition

Return the position on the path according to the distance with path starting point
(see also Segment::GetPosition)
===============================================================================================
*/
glm::vec2 Path::GetPosition(float distance)
{
	float cumulativeDistance = 0.0f;

	for (auto segment : segments) {
		float segmentLength = segment.GetLength();

		if (cumulativeDistance + segmentLength < distance) {
			cumulativeDistance += segment.GetLength();
		}
		else {
			return segment.GetPosition(distance - cumulativeDistance);
		}
	}
	std::cerr << "[Path::GetPosition] Passed parameter distance should lay " <<
				 "between 0 and " << cumulativeDistance << std::endl;
	return Segment::OUTSIDE_SEGMENT;
}