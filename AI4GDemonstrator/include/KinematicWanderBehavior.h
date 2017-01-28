#pragma once

#include "KinematicMovementBehavior.h"
#include <random>

class KinematicWanderBehavior : public KinematicMovementBehavior
{
public:
	KinematicWanderBehavior(SpatialStructure* character, float speed, float rotation);
	~KinematicWanderBehavior();

	KinematicSteeringOutput ComputeMovement();

private:
	float maxSpeed;
	float maxRotation;

	static std::random_device rd;
	static std::mt19937 engine;
	static std::uniform_real_distribution<float> distr;
};