#pragma once

#include "SteeringAlignBehavior.h"

class SteeringFaceBehavior : public SteeringAlignBehavior
{
public:
									SteeringFaceBehavior(SpatialStructure* character, SpatialStructure* target);
	virtual							~SteeringFaceBehavior();

	virtual KinematicSteeringOutput ComputeMovement() override;

	virtual std::string				ToString()					{ return "SteeringFaceBehavior"; }
};