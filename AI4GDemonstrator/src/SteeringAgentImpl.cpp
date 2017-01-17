#include "SteeringAgentImpl.h"
#include <gtc/constants.hpp>

SteeringAgentImpl::SteeringAgentImpl(SteeringMovementBehavior* behavior, float speed) : movementBehavior(behavior), maxSpeed(speed)
{}

SteeringAgentImpl::~SteeringAgentImpl()
{}

void SteeringAgentImpl::Update(float deltaTime)
{
	auto transform = movementBehavior->GetCharacterPtr();
	transform->position += transform->velocity * deltaTime;
	transform->orientation += transform->rotation * deltaTime;
	transform->orientation = fmodf(transform->orientation, glm::two_pi<float>());

	auto movement = movementBehavior->ComputeMovement();
	transform->velocity += movement.linear * deltaTime;

	if (glm::length(transform->velocity) > maxSpeed) {
		transform->velocity = glm::normalize(transform->velocity) * maxSpeed;
	}
	transform->rotation += movement.angular * deltaTime;
}