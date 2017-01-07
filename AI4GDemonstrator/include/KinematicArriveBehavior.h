#pragma once

#include "KinematicMovementBehavior.h"

class KinematicArriveBehavior : public KinematicMovementBehavior
{
public:
	KinematicArriveBehavior(SpatialStructure* character, SpatialStructure* target, float radius, float time, float speed = 0.0f);
	~KinematicArriveBehavior();

	KinematicSteeringOutput ComputeMovement();

private:
	float radius;
	float timeToArrive;
	float maxSpeed;
};