#pragma once

#include "SteeringFleeBehavior.h"

class SteeringEvadeBehavior : public SteeringFleeBehavior
{
public:
	SteeringEvadeBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f, float prediction = 0.5f);
	~SteeringEvadeBehavior();

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringEvadeBehavior";
	}

private:
	float maxPrediction;
};