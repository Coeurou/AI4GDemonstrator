#pragma once

#include <memory>
#include "SpatialStructure.h"
#include "AgentImpl.h"

class Agent
{
public:
	Agent(std::unique_ptr<AgentImpl> agentImpl) : impl(std::move(agentImpl)) {}
	Agent(AgentImpl* agentImpl) : impl(agentImpl) {}
	Agent(const Agent& toCopy) = delete;
	Agent(Agent&& rval) noexcept : impl(std::move(rval.impl)) {}
	~Agent() {}

	Agent& operator=(const Agent& toCopy) = delete;
	Agent& operator=(Agent&& rval) noexcept { impl = std::move(rval.impl); return *this; }

	void UpdateAgent(float deltaTime) { impl->Update(deltaTime); }
	void SetAgentImpl(AgentImpl* agentImpl) { impl.reset(agentImpl); }
	void SetAgentImpl(std::unique_ptr<AgentImpl> agentImpl) { impl = std::move(agentImpl); }

private:
	std::unique_ptr<AgentImpl> impl;
};