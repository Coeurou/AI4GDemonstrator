#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringSeekBehavior : public SteeringMovementBehavior
{
public:
	SteeringSeekBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f);
	~SteeringSeekBehavior();

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringSeekBehavior";
	}

protected:
	float GetMaxAcceleration() const { return maxAcceleration; }
	void SetMaxAcceleration(float acc) { maxAcceleration = acc; }

private:
	float maxAcceleration;
};