#pragma once

#include <memory>
#include "AgentImpl.h"
#include "KinematicMovementBehavior.h"

class KinematicAgentImpl : public AgentImpl
{
public:
	KinematicAgentImpl(KinematicMovementBehavior* behavior);
	~KinematicAgentImpl();

	void Update();
	glm::vec2 GetPosition() const { return movementBehavior->GetCharacter().position; }
	void SetTarget(SpatialStructure* target) { movementBehavior->SetTarget(target); }

private:
	std::unique_ptr<KinematicMovementBehavior> movementBehavior;
};