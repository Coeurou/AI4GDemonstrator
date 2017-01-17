#pragma once

#include <memory>
#include "AgentImpl.h"
#include "SteeringMovementBehavior.h"

class SteeringAgentImpl : public AgentImpl
{
public:
	SteeringAgentImpl(SteeringMovementBehavior* behavior, float speed);
	SteeringAgentImpl(SteeringMovementBehavior&& behavior, float speed);
	~SteeringAgentImpl();

	void Update(float deltaTime) override;
	glm::vec2 GetPosition() const override { return movementBehavior->GetCharacter().position; }
	void SetTarget(SpatialStructure* target) { movementBehavior->SetTarget(target); }

private:
	std::unique_ptr<SteeringMovementBehavior> movementBehavior;
	float maxSpeed;
};
