#include "Segment.h"

const glm::vec2 Segment::OUTSIDE_SEGMENT = glm::vec2(std::numeric_limits<float>::infinity());
const float		Segment::DISTANCE_NOT_FOUND = -1.0f;


glm::vec2 Segment::GetSegment() const
{
	return points[1] - points[0];
}

float Segment::GetLength() const
{
	return glm::length(GetSegment());
}

/*
===============================================================================================
GetNeareastPoint

Return the nearest point between a position and the segment.
(see also http://geomalgorithms.com/a02-_lines.html -> Distance of a point to a ray or segment)
===============================================================================================
*/
glm::vec2 Segment::GetNeareastPoint(const glm::vec2& pos) const
{
	glm::vec2 segment = glm::normalize(GetSegment());

	glm::vec2 ptAToPos = pos - points[0];
	float dotPosPtA = glm::dot(ptAToPos, segment);
	if (dotPosPtA <= 0.0f) {
		return points[0];
	}

	float dotSegment = glm::dot(segment, segment);
	if (dotSegment <= dotPosPtA) {
		return points[1];
	}

	float lerp = dotPosPtA / dotSegment;
	return points[0] + lerp * segment;
}

/*
===============================================================================================
GetPosition

Return a point on the segment according to the factor passed as parameter.
Parameter distance should lay between 0 and 1, if not the point returned is not on the segment.
===============================================================================================
*/
glm::vec2 Segment::GetPosition(float distance) const
{
	if (distance < 0.0f || distance > 1.0f) {		
		return OUTSIDE_SEGMENT;
	}
	return GetSegment() * distance;
}

/*
===============================================================================================
GetDistance

Return the distance between segment first point and pos parameter, this works
only if pos and the segment are colinear (length[A;pos] + length[b;pos] == length[a;b]).
===============================================================================================
*/
float Segment::GetDistance(const glm::vec2& pos) const
{
	float lengthPosPtA = glm::length(pos - points[0]);
	float lengthPosPtB = glm::length(pos - points[1]);

	if (lengthPosPtA + lengthPosPtB == GetLength())
	{
		return lengthPosPtA;
	}

	return DISTANCE_NOT_FOUND;
}
