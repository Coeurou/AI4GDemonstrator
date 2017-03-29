#pragma once
#include "SteeringMovementBehavior.h"
#include "BlendedBehavior.h"
#include <vector>

class PrioritySteering : public SteeringMovementBehavior
{
public:
									PrioritySteering(const std::vector<BlendedBehavior*>& behaviors, float threshold);
									PrioritySteering(std::initializer_list<BlendedBehavior*> behaviors, float threshold);
									~PrioritySteering();

	KinematicSteeringOutput			ComputeMovement() override;
	std::string						ToString() override { return "PrioritySteering"; }

private:
	std::vector<BlendedBehavior*>	groups; // Order matter
	float							epsilon;
};