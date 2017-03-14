#pragma once

#include "SteeringSeekBehavior.h"
#include "Path.h"

class SteeringPathFollowingBehavior : public SteeringSeekBehavior
{

public:
							SteeringPathFollowingBehavior(SpatialStructure* character, SpatialStructure* target, Path path,
														  float speed = 1.0f, float offset = 1.0f, float prediction = 0.5f);
							~SteeringPathFollowingBehavior();

	KinematicSteeringOutput ComputeMovement();

	virtual std::string		ToString();

private:
	float					predictionTime;
	float					targetOffset;
	float					currentDistOnPath;
	Path					path;
};