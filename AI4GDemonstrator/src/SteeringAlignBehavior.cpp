#include "SteeringAlignBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>
#include <gtc/constants.hpp>

SteeringAlignBehavior::SteeringAlignBehavior(SpatialStructure* character, SpatialStructure* target)
	: SteeringMovementBehavior(character, target), maxAcceleration(1.0f), maxSpeed(1.0f), targetRadius(0.2f), slowRadius(1.5f), timeToArrive(0.1f)
{}

SteeringAlignBehavior::~SteeringAlignBehavior()
{}

float MapToRange(float val, float min, float max)
{
	float mod = fmodf(val + min, max);
	if (mod < 0) {
		mod += max;
	}
	return mod - min;
}

KinematicSteeringOutput SteeringAlignBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	auto rotation = target->orientation - character->orientation;
	rotation = MapToRange(rotation, glm::pi<float>(), glm::two_pi<float>());
	float rotationSize = abs(rotation);


	movement.linear = glm::vec2(0);
	if (rotationSize < targetRadius) {
		movement.angular = 0;
		return movement;
	}

	float targetSpeed = (rotationSize > slowRadius) ? maxSpeed : maxSpeed * rotationSize / slowRadius;

	rotation = glm::normalize(rotation) * targetSpeed;
	movement.angular = rotation - character->rotation;
	movement.angular /= timeToArrive;

	if (glm::length(movement.angular) > maxAcceleration) {
		movement.angular = glm::normalize(movement.angular) * maxAcceleration;
	}

	return movement;
}