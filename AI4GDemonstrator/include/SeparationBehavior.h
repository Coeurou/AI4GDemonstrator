#pragma once

#include "SteeringMovementBehavior.h"
#include <vector>

class SeparationBehavior : public SteeringMovementBehavior
{
public:
									SeparationBehavior(SpatialStructure* character, std::initializer_list<SpatialStructure*> targets,
													   float minDistance, float decay, float maxAcc);
									SeparationBehavior(SpatialStructure* character, const std::vector<SpatialStructure*>& targets,
													   float minDistance, float decay, float maxAcc);
									~SeparationBehavior();

	KinematicSteeringOutput			ComputeMovement() override;
	std::string						ToString()		  override { return "SeparationBehavior"; }

private:
	std::vector<SpatialStructure*>	targets;
};