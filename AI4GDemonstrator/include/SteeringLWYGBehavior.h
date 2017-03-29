#pragma once

#include "SteeringAlignBehavior.h"

class SteeringLWYGBehavior : public SteeringAlignBehavior
{
public:
							SteeringLWYGBehavior(SpatialStructure* character);
							~SteeringLWYGBehavior();

	KinematicSteeringOutput ComputeMovement() override;

	virtual std::string		ToString()					{ return "SteeringLWYGBehavior"; }
};