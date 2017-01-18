#pragma once

#include "SteeringAlignBehavior.h"

class SteeringFaceBehavior : public SteeringAlignBehavior
{
public:
	SteeringFaceBehavior(SpatialStructure* character, SpatialStructure* target);
	~SteeringFaceBehavior();

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringFaceBehavior";
	}
};