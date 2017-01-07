#pragma once

#include "KinematicMovementBehavior.h"

class KinematicFleeBehavior : public KinematicMovementBehavior
{
public:
	KinematicFleeBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f);
	~KinematicFleeBehavior();

	KinematicSteeringOutput ComputeMovement();

private:
	float maxSpeed;
};