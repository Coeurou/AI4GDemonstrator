#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringArriveBehavior : public SteeringMovementBehavior
{
public:
							SteeringArriveBehavior(SpatialStructure* character, SpatialStructure* target);
							~SteeringArriveBehavior();

	KinematicSteeringOutput ComputeMovement() override;

	virtual std::string		ToString()						{ return "SteeringArriveBehavior"; }
};