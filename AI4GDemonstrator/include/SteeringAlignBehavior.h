#pragma once

#include "SteeringMovementBehavior.h"
#include "SpatialStructure.h"

class SteeringAlignBehavior : public SteeringMovementBehavior
{
public:
	SteeringAlignBehavior(SpatialStructure* character, SpatialStructure* target);
	~SteeringAlignBehavior();
		
	void SetMaxAcceleration(float acc) { maxRotation = acc; }
	void SetMaxSpeed(float speed)	   { maxSpeed = speed; }
	void SetTargetRadius(float radius) { targetRadius = radius; }
	void SetSlowRadius(float radius)   { slowRadius = radius; }
	void SetTimeToArrive(float time)   { timeToArrive = time; }

	KinematicSteeringOutput ComputeMovement();

	virtual std::string ToString()
	{
		return "SteeringAlignBehavior";
	}

private:
	float maxRotation;
	float maxSpeed;
	float targetRadius;
	float slowRadius;
	float timeToArrive;
};