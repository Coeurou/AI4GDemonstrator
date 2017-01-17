#include "SteeringArriveBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>

SteeringArriveBehavior::SteeringArriveBehavior(SpatialStructure* character, SpatialStructure* target)
	: SteeringMovementBehavior(character, target), maxAcceleration(1.0f), maxSpeed(1.0f), targetRadius(0.2f), slowRadius(1.5f), timeToArrive(0.1f)
{}

SteeringArriveBehavior::~SteeringArriveBehavior()
{}

KinematicSteeringOutput SteeringArriveBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	auto direction = target->position - character->position;
	float distance = glm::length(direction);

	movement.angular = 0;
	if (distance < targetRadius) {
		movement.linear = glm::vec2(0);
		return movement;
	}

	float targetSpeed = (distance > slowRadius) ? maxSpeed : maxSpeed * distance / slowRadius;

	direction = glm::normalize(direction) * targetSpeed;
	movement.linear = direction - character->velocity;
	movement.linear /= timeToArrive;

	if (glm::length(movement.linear) > maxAcceleration) {
		movement.linear = glm::normalize(movement.linear) * maxAcceleration;
	}

	return movement;
}