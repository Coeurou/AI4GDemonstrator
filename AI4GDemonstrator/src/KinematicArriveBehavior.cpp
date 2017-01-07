#include "KinematicArriveBehavior.h"
#include "SpatialStructureUtility.h"

KinematicArriveBehavior::KinematicArriveBehavior(SpatialStructure* character, SpatialStructure* target, float radius, float time, float speed)
	: KinematicMovementBehavior(character, target), radius(radius), timeToArrive(time), maxSpeed(speed)
{}

KinematicArriveBehavior::~KinematicArriveBehavior()
{}

KinematicSteeringOutput KinematicArriveBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	movement.linear = target->position - character->position;
	movement.angular = GetNewOrientation(movement.linear, character->orientation);

	if (glm::length(movement.linear) < radius) {
		movement.linear = glm::vec2(0);
		return movement;
	}

	movement.linear /= timeToArrive;
	if (glm::length(movement.linear) > maxSpeed) {
		movement.linear = glm::normalize(movement.linear) * maxSpeed;
	}

	return movement;
}