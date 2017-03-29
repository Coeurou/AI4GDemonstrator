#pragma once

#include "SteeringSeekBehavior.h"
#include "Path.h"

class SteeringPathFollowingBehavior : public SteeringSeekBehavior
{

public:
							SteeringPathFollowingBehavior(SpatialStructure* character, SpatialStructure* target, Path path,
														  float speed = 1.0f, float offset = 1.0f, float prediction = 0.5f);
							~SteeringPathFollowingBehavior();

	KinematicSteeringOutput ComputeMovement() override;

	virtual std::string		ToString()							{ return "SteeringPathFollowingBehavior"; }

private:
	float					currentDistOnPath;
	Path					path;
};