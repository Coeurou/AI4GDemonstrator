// main.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>
#include <chrono>
#include "Agent.h"
#include "SteeringAgentImpl.h"
#include "SpatialStructure.h"
#include "SpatialStructureUtility.h"
#include "SteeringSeekBehavior.h"

int main()
{
	SpatialStructure character;
	SpatialStructure target;
	target.position = glm::vec2(50);

	float speed = 5.0f;

	Agent agent(new SteeringAgentImpl(new SteeringSeekBehavior(&character, &target, speed), speed));

	auto start = std::chrono::system_clock::now();
	while (glm::length(target.position - character.position) > 0.05f * speed)
	{
		std::chrono::duration<float> elapsedTime = std::chrono::system_clock::now() - start;
		agent.UpdateAgent(elapsedTime.count());
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
			"************************************************************************** \n" << std::endl;
	}

	int e = 0;
	std::cin >> e;
	return 0;
}