#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringSeekBehavior : public SteeringMovementBehavior
{
public:
	SteeringSeekBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f);
	~SteeringSeekBehavior();

	KinematicSteeringOutput ComputeMovement();

private:
	float maxAcceleration;
};