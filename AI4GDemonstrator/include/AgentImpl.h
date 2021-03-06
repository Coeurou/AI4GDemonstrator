#pragma once

#include <glm.hpp>
#include "SpatialStructure.h"

class AgentImpl
{
public:
	AgentImpl() {}
	virtual ~AgentImpl() {}

	virtual void Update(float deltaTime) = 0;
	virtual glm::vec2 GetPosition() const = 0;
};