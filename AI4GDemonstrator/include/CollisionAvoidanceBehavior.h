#pragma once

#include "SteeringMovementBehavior.h"
#include <vector>

class CollisionAvoidanceBehavior : public SteeringMovementBehavior
{
public:
									CollisionAvoidanceBehavior(SpatialStructure* character, const std::vector<SpatialStructure*>& targets, float radius, float acceleration);
									~CollisionAvoidanceBehavior();

	KinematicSteeringOutput			ComputeMovement() override;
	std::string						ToString()		  override { return "CollisionAvoidanceBehavior"; }

private:
	std::vector<SpatialStructure*>	targets;
};