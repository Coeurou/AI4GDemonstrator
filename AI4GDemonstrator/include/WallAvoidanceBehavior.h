#pragma once

#include "SteeringSeekBehavior.h"
#include "AABB.h"
#include "CollisionDetector.h"
#include <vector>

class CollisionDetector;

class WallAvoidanceBehavior : public SteeringSeekBehavior
{
public:
									WallAvoidanceBehavior(SpatialStructure* character,
														  const std::vector<AABB>& targets,
														  float speed, float evadeDistance, float rayLength);
									~WallAvoidanceBehavior();

	KinematicSteeringOutput			ComputeMovement() override;
	std::string						ToString()		  override															{ return "WallAvoidanceBehavior"; }

private:
	std::vector<AABB>				targets;
	CollisionDetector				collisionDetector;
};