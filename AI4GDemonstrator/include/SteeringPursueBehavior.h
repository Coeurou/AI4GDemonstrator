#pragma once

#include "SteeringSeekBehavior.h"

class SteeringPursueBehavior : public SteeringSeekBehavior
{
public:
									SteeringPursueBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f, float prediction = 0.5f);
									~SteeringPursueBehavior();

	virtual KinematicSteeringOutput ComputeMovement() override;

	virtual	std::string				ToString()					{ return "SteeringPursueBehavior"; }

private:
	float							maxPrediction;
};