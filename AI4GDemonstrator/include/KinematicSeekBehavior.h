#pragma once

#include "KinematicMovementBehavior.h"
#include "SpatialStructure.h"

class KinematicSeekBehavior : public KinematicMovementBehavior
{
public:
	KinematicSeekBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f);
	~KinematicSeekBehavior();

	KinematicSteeringOutput ComputeMovement();

private:
	float maxSpeed;
};