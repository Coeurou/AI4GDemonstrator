#pragma once

#include "SteeringFaceBehavior.h"
#include <random>

class SteeringWanderBehavior : public SteeringFaceBehavior
{
public:
													SteeringWanderBehavior(SpatialStructure* character, float acc = 1.0f, float radius = 1.0f,
																		   float offset = 1.0f, float rate = 1.0f);
													~SteeringWanderBehavior();

	KinematicSteeringOutput							ComputeMovement() override;

	virtual std::string								ToString()					{ return "SteeringWanderBehavior"; }

private:
	float											wanderOrientation;

	static std::random_device						rd;
	static std::mt19937								engine;
	static std::uniform_real_distribution<float>	distr;
};