#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringFleeBehavior : public SteeringMovementBehavior
{
public:
	SteeringFleeBehavior(SpatialStructure* character, SpatialStructure* target, float speed = 1.0f);
	~SteeringFleeBehavior();

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringFleeBehavior";
	}

protected:
	float GetMaxAcceleration() const { return maxAcceleration; }
	void SetMaxAcceleration(float acc) { maxAcceleration = acc; }

private:
	float maxAcceleration;
};