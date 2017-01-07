#pragma once

#include "KinematicSteeringOutput.h"
#include "SpatialStructure.h"

class KinematicMovementBehavior
{
public:
	KinematicMovementBehavior(SpatialStructure* character, SpatialStructure* target) : character(character), target(target) {}
	virtual ~KinematicMovementBehavior() {}

	virtual KinematicSteeringOutput ComputeMovement() { KinematicSteeringOutput empty; return empty; }

	SpatialStructure GetCharacter() const { return *character; }
	SpatialStructure GetTarget() const { return *target; }
	void SetTarget(SpatialStructure* target) { this->target = target; }

	SpatialStructure* GetCharacterPtr() { return character; }
	SpatialStructure* GetTargetPtr() { return target; }

private:
	SpatialStructure* character;
	SpatialStructure* target;
};