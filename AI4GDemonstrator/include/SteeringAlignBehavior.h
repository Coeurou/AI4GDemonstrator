#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringAlignBehavior : public SteeringMovementBehavior
{
public:
									SteeringAlignBehavior(SpatialStructure* character, SpatialStructure* target);
	virtual							~SteeringAlignBehavior();

	virtual KinematicSteeringOutput ComputeMovement() override;

	virtual std::string				ToString()								{ return "SteeringAlignBehavior"; }
};