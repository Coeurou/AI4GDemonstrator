#include "SteeringFleeBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>

SteeringFleeBehavior::SteeringFleeBehavior(SpatialStructure* character, SpatialStructure* target, float speed)
	: SteeringMovementBehavior(character, target), maxAcceleration(speed)
{}

SteeringFleeBehavior::~SteeringFleeBehavior()
{}

KinematicSteeringOutput SteeringFleeBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	movement.linear = character->position - target->position;
	movement.linear = glm::normalize(movement.linear) * maxAcceleration;
	movement.angular = 0;

	return movement;
}