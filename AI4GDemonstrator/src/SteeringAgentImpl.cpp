#include "SteeringAgentImpl.h"
#include <gtc/constants.hpp>

SteeringAgentImpl::SteeringAgentImpl(SteeringMovementBehavior* behavior, float speed) : movementBehavior(std::move(behavior)), maxSpeed(speed)
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
	transform->velocity = glm::min(transform->velocity, glm::vec2(1) * maxSpeed);
	transform->rotation += movement.angular * deltaTime;
}