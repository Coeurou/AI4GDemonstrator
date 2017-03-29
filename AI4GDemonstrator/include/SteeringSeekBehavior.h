#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringSeekBehavior : public SteeringMovementBehavior
{
public:
									SteeringSeekBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f);
	virtual							~SteeringSeekBehavior();

	virtual KinematicSteeringOutput ComputeMovement() override;

	virtual std::string				ToString()						{ return "SteeringSeekBehavior"; }

};