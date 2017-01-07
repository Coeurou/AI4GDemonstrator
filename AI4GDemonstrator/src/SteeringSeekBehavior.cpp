#include "SteeringSeekBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>

SteeringSeekBehavior::SteeringSeekBehavior(SpatialStructure* character, SpatialStructure* target, float speed)
	: SteeringMovementBehavior(character, target), maxAcceleration(speed)
{}

SteeringSeekBehavior::~SteeringSeekBehavior()
{}

KinematicSteeringOutput SteeringSeekBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	movement.linear = target->position - character->position;
	movement.linear = glm::normalize(movement.linear) * maxAcceleration;
	movement.angular = 0;

	return movement;
}