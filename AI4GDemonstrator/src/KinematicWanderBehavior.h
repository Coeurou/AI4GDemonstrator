#pragma once

#include "KinematicMovementBehavior.h"

class KinematicWanderBehavior : public KinematicMovementBehavior
{
public:
	KinematicWanderBehavior(SpatialStructure* character, float speed, float rotation);
	~KinematicWanderBehavior();

	KinematicSteeringOutput ComputeMovement();

private:
	float maxSpeed;
	float maxRotation;
};