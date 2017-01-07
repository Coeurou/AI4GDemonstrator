#include "KinematicFleeBehavior.h"
#include "SpatialStructureUtility.h"

KinematicFleeBehavior::KinematicFleeBehavior(SpatialStructure* character, SpatialStructure* target, float speed) 
	: KinematicMovementBehavior(character, target), maxSpeed(speed)
{}

KinematicFleeBehavior::~KinematicFleeBehavior()
{}

KinematicSteeringOutput KinematicFleeBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;
	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	movement.linear = character->position - target->position;
	movement.linear = glm::normalize(movement.linear) * maxSpeed;
	movement.angular = GetNewOrientation(movement.linear, character->orientation);

	return movement;
}