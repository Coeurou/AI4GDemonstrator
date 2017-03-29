#include "PrioritySteering.h"

PrioritySteering::PrioritySteering(const std::vector<BlendedBehavior*>& behaviors, float threshold)
	: SteeringMovementBehavior(nullptr, nullptr), groups(behaviors), epsilon(threshold)
{}

PrioritySteering::PrioritySteering(std::initializer_list<BlendedBehavior*> behaviors, float threshold)
	: SteeringMovementBehavior(nullptr, nullptr), groups(behaviors), epsilon(threshold)
{}

PrioritySteering::~PrioritySteering()
{}

KinematicSteeringOutput PrioritySteering::ComputeMovement()
{
	KinematicSteeringOutput movement;

	for (auto group : groups) {
		movement =  group->ComputeMovement();
		if (glm::length(movement.linear) > epsilon ||
			std::abs(movement.angular) > epsilon) {
			return movement;
		}
	}

	return movement;
}
