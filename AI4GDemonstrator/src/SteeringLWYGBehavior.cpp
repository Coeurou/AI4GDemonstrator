#include "SteeringLWYGBehavior.h"
#include "SpatialStructureUtility.h"


/*==============================================================================================
							  Looking where you going behavior
================================================================================================*/
SteeringLWYGBehavior::SteeringLWYGBehavior(SpatialStructure* character)
	: SteeringAlignBehavior(character, nullptr)
{}

SteeringLWYGBehavior::~SteeringLWYGBehavior()
{}

KinematicSteeringOutput SteeringLWYGBehavior::ComputeMovement()
{
	auto character = GetCharacterPtr();
	SpatialStructure target;

	target.orientation = GetNewOrientation(character->velocity, character->orientation);
	SetTarget(&target);

	return SteeringAlignBehavior::ComputeMovement();
}