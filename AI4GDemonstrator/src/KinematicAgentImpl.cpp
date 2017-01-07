#include "KinematicAgentImpl.h"
#include <gtc/constants.hpp>

KinematicAgentImpl::KinematicAgentImpl(KinematicMovementBehavior* behavior)	: movementBehavior(std::move(behavior))
{}

KinematicAgentImpl::~KinematicAgentImpl()
{}

void KinematicAgentImpl::Update()
{
	auto movement = movementBehavior->ComputeMovement();
	auto transform = movementBehavior->GetCharacterPtr();
	transform->position += movement.linear;
	transform->orientation = fmodf(movement.angular, glm::two_pi<float>());
}
