#include "SteeringFaceBehavior.h"
#include "SpatialStructureUtility.h"

SteeringFaceBehavior::SteeringFaceBehavior(SpatialStructure* character, SpatialStructure* target)
	: SteeringAlignBehavior(character, target)
{}

SteeringFaceBehavior::~SteeringFaceBehavior()
{}

KinematicSteeringOutput SteeringFaceBehavior::ComputeMovement()
{
	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	auto direction = target->position - character->position;
	target->orientation = GetNewOrientation(direction, character->orientation);

	return SteeringAlignBehavior::ComputeMovement();
}