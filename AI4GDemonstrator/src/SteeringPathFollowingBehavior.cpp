#include "SteeringPathFollowingBehavior.h"

SteeringPathFollowingBehavior::SteeringPathFollowingBehavior(SpatialStructure* character, SpatialStructure* target,
															 Path path, float speed, float offset, float prediction)
	: SteeringSeekBehavior(character, target, speed), path(path), targetOffset(offset), predictionTime(prediction)
{}

SteeringPathFollowingBehavior::~SteeringPathFollowingBehavior()
{}

/*
===============================================================================================
ComputeMovement

Compute the target passed to seek behavior based on character position and the path to follow.
(see also SteerigSeekBehavior or chapter 3.3.12 AI4G, 2nd edition)
===============================================================================================
*/
KinematicSteeringOutput SteeringPathFollowingBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	glm::vec2 predictedPosition = character->position + character->velocity * predictionTime;
	float newDistance = path.GetDistance(predictedPosition);
	if (newDistance != Segment::DISTANCE_NOT_FOUND) {
		currentDistOnPath = newDistance;
	}

	float targetDistance = currentDistOnPath + targetOffset;

	auto nextTarget = path.GetPosition(targetDistance);
	target->position = (nextTarget != Segment::OUTSIDE_SEGMENT) ? nextTarget : character->position;

	return SteeringSeekBehavior::ComputeMovement();
}

std::string SteeringPathFollowingBehavior::ToString()
{
	return "SteeringPathFollowingBehavior";
}