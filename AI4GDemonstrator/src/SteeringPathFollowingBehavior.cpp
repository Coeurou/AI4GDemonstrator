#include "SteeringPathFollowingBehavior.h"

SteeringPathFollowingBehavior::SteeringPathFollowingBehavior(SpatialStructure* character, SpatialStructure* target,
															 Path path, float speed, float offset, float prediction)
	: SteeringSeekBehavior(character, target, speed), path(path)
{
	auto& parameters = GetParametersByRef();
	parameters[DISTANCE_PARAM] = { Parameter::FLOAT, offset };
	parameters[MAXPREDICTION_PARAM] = { Parameter::FLOAT, prediction };
}

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
	auto& parameters = GetParametersByRef();
	auto predictionTime = parameters[MAXPREDICTION_PARAM].floatValue;
	auto targetOffset = parameters[DISTANCE_PARAM].floatValue;

	glm::vec2 predictedPosition = character->position + character->velocity * predictionTime;
	float newDistance = path.GetDistance(predictedPosition);
	if (newDistance != Segment::DISTANCE_NOT_FOUND) {
		currentDistOnPath = newDistance;
	}

	float targetDistance = currentDistOnPath + targetOffset;

	auto nextTarget = path.GetPosition(targetDistance);

	SpatialStructure target;
	target.position = (nextTarget != Segment::OUTSIDE_SEGMENT) ? nextTarget : character->position;
	SetTarget(&target);

	return SteeringSeekBehavior::ComputeMovement();
}