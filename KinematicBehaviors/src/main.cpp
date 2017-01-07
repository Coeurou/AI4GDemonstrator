// main.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>
#include <chrono>
#include "Agent.h"
#include "KinematicAgentImpl.h"
#include "SpatialStructure.h"
#include "SpatialStructureUtility.h"
#include "KinematicSeekBehavior.h"
#include "KinematicArriveBehavior.h"
#include "KinematicWanderBehavior.h"
#include "KinematicSeekBehavior.h"
#include "KinematicFleeBehavior.h"

int main()
{
	SpatialStructure character;
	SpatialStructure target;
	target.position = glm::vec2(50);

	float speed = 5.0f;
		
	Agent agent(new KinematicAgentImpl(new KinematicSeekBehavior(&character, &target, speed)));

	while (glm::length(target.position - character.position) > 0.5f * speed)
	{
		agent.UpdateAgent(0);
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " ---------> TARGET: " << 
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
					 "************************************************************************** \n" << std::endl;
	}

	agent.SetAgentImpl(new KinematicAgentImpl(new KinematicFleeBehavior(&character, &target, speed)));
	while (glm::length(target.position - character.position) < 14.0f * speed)
	{
		agent.UpdateAgent(0);
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
			"************************************************************************** \n" << std::endl;
	}

	float timeToArrive = 0.75f;
	float radius = speed * 0.05f;
	agent.SetAgentImpl(new KinematicAgentImpl(new KinematicArriveBehavior(&character, &target, radius, timeToArrive, speed)));

	bool arrived = false;
	while (!arrived)
	{
		arrived = (glm::length(target.position - character.position) < 0.5f);

		agent.UpdateAgent(0);
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " Direction: " << character.orientation << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
			"*************************************************************************** \n" << std::endl;
	}

	std::chrono::duration<int> one_second(1);
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + one_second;

	agent.SetAgentImpl(new KinematicAgentImpl(new KinematicWanderBehavior(&character, speed, 7.0f)));
	while (std::chrono::system_clock::now() < end)
	{
		agent.UpdateAgent(0);
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " Direction: " << character.orientation << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
			"*************************************************************************** \n" << std::endl;
	}

	int e = 0;
	std::cin >> e;
    return 0;
}