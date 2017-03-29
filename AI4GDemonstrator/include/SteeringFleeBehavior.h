#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringFleeBehavior : public SteeringMovementBehavior
{
public:
							SteeringFleeBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f);
							~SteeringFleeBehavior();

	virtual KinematicSteeringOutput ComputeMovement() override;

	virtual std::string		ToString()								{ return "SteeringFleeBehavior"; }
};