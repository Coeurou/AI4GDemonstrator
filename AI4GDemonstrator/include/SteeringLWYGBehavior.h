#pragma once

#include "SteeringAlignBehavior.h"

class SteeringLWYGBehavior : public SteeringAlignBehavior
{
public:
	SteeringLWYGBehavior(SpatialStructure* character, SpatialStructure* target);
	~SteeringLWYGBehavior();

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringLWYGBehavior";
	}
};