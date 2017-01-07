#pragma once

#include "KinematicSteeringOutput.h"
#include "SpatialStructure.h"

class SteeringMovementBehavior
{
public:
	SteeringMovementBehavior(SpatialStructure* character, SpatialStructure* target) : character(character), target(target) {}
	virtual ~SteeringMovementBehavior() {}

	virtual KinematicSteeringOutput ComputeMovement() { return KinematicSteeringOutput(); }

	SpatialStructure GetCharacter() const { return *character; }
	SpatialStructure GetTarget() const { return *target; }
	void SetTarget(SpatialStructure* target) { this->target = target; }

	SpatialStructure* GetCharacterPtr() { return character; }
	SpatialStructure* GetTargetPtr() { return target; }

private:
	SpatialStructure* character;
	SpatialStructure* target;
};