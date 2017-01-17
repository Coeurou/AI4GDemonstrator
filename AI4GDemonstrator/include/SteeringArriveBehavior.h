#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringArriveBehavior : public SteeringMovementBehavior
{
public:
	SteeringArriveBehavior(SpatialStructure* character, SpatialStructure* target);
	~SteeringArriveBehavior();

	void SetMaxAcceleration(float acc) { maxAcceleration = acc; }
	void SetMaxSpeed(float speed)	   { maxSpeed = speed; }
	void SetTargetRadius(float radius) { targetRadius = radius; }
	void SetSlowRadius(float radius)   { slowRadius = radius; }
	void SetTimeToArrive(float time)   { timeToArrive = time; }

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringArriveBehavior";
	}

private:
	float maxAcceleration;
	float maxSpeed;
	float targetRadius;
	float slowRadius;
	float timeToArrive;
};