#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringVelocityMatchingBehavior : public SteeringMovementBehavior
{
public:
	SteeringVelocityMatchingBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f, float time = 0.1f);
	~SteeringVelocityMatchingBehavior();

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringVelocityMatchingBehavior";
	}

private:
	float maxAcceleration;
	float timeToTarget;
};