#include "SteeringLWYGBehavior.h"
#include "SpatialStructureUtility.h"


/*==============================================================================================
							  Looking where you going behavior
================================================================================================*/
SteeringLWYGBehavior::SteeringLWYGBehavior(SpatialStructure* character, SpatialStructure* target)
	: SteeringAlignBehavior(character, target)
{}

SteeringLWYGBehavior::~SteeringLWYGBehavior()
{}

KinematicSteeringOutput SteeringLWYGBehavior::ComputeMovement()
{
	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	target->orientation = GetNewOrientation(character->velocity, character->orientation);

	return SteeringAlignBehavior::ComputeMovement();
}