#pragma once

#include <memory>
#include "AgentImpl.h"
#include "KinematicMovementBehavior.h"

class KinematicAgentImpl : public AgentImpl
{
public:
	KinematicAgentImpl(KinematicMovementBehavior* behavior);
	~KinematicAgentImpl();

	void Update(float deltaTime) override;
	glm::vec2 GetPosition() const override { return movementBehavior->GetCharacter().position; } 
	void SetTarget(SpatialStructure* target) { movementBehavior->SetTarget(target); }

private:
	std::unique_ptr<KinematicMovementBehavior> movementBehavior;
};