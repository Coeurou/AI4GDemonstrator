#include "KinematicSeekBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>

KinematicSeekBehavior::KinematicSeekBehavior(SpatialStructure* character, SpatialStructure* target, float speed)
	: KinematicMovementBehavior(character, target), maxSpeed(speed)
{}

KinematicSeekBehavior::~KinematicSeekBehavior()
{}

KinematicSteeringOutput KinematicSeekBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	movement.linear = target->position - character->position;
	movement.linear = glm::normalize(movement.linear) * maxSpeed;
	movement.angular = GetNewOrientation(movement.linear, character->orientation);

	return movement;
}