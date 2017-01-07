#pragma once

#include <memory>
#include "SpatialStructure.h"
#include "AgentImpl.h"

class Agent
{
public:
	Agent(std::unique_ptr<AgentImpl> agentImpl) : impl(std::move(agentImpl)) {}
	Agent(AgentImpl* agentImpl) : impl(agentImpl) {}
	~Agent() {}

	void UpdateAgent() { impl->Update(); }
	void SetAgentImpl(AgentImpl* agentImpl) { impl.reset(agentImpl); }
	void SetAgentImpl(std::unique_ptr<AgentImpl> agentImpl) { impl = std::move(agentImpl); }

private:
	std::unique_ptr<AgentImpl> impl;
};