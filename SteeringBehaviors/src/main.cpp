// main.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>
#include <chrono>
#include "Agent.h"
#include "SteeringAgentImpl.h"
#include "SpatialStructure.h"
#include "SpatialStructureUtility.h"
#include "SteeringSeekBehavior.h"
#include "SteeringFleeBehavior.h"
#include "SteeringArriveBehavior.h"
#include "SteeringAlignBehavior.h"
#include "SteeringPursueBehavior.h"
#include "KinematicWanderBehavior.h"
#include "KinematicAgentImpl.h"

int main()
{
	SpatialStructure character;
	SpatialStructure target;
	target.position = glm::vec2(50);

	float speed = 5.0f;

	Agent agent(new SteeringAgentImpl(new SteeringSeekBehavior(&character, &target, speed), speed));

	auto start = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime;
	
	std::cout << "**********************************START SEEK********************************\n" << std::endl;
	while (glm::length(target.position - character.position) > 0.05f * speed)
	{
		elapsedTime = std::chrono::system_clock::now() - start;
		agent.UpdateAgent(elapsedTime.count());
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
			"************************************************************************** \n" << std::endl;
	}

	std::cout << "**********************************START FLEE********************************\n" << std::endl;
	agent.SetAgentImpl(new SteeringAgentImpl(new SteeringFleeBehavior(&character, &target, speed), speed)); 
	while (glm::length(target.position - character.position) < 14.0f * speed)
	{
		elapsedTime = std::chrono::system_clock::now() - start;
		agent.UpdateAgent(elapsedTime.count());
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
			"************************************************************************** \n" << std::endl;
	}

	std::cout << "*********************************START ARRIVE*******************************\n" << std::endl;
	float timeToArrive = 0.75f;
	float radius = speed * 0.05f;
	auto arriveBehavior = new SteeringArriveBehavior(&character, &target);
	arriveBehavior->SetMaxAcceleration(3.0f);
	arriveBehavior->SetMaxSpeed(3.0f);
	arriveBehavior->SetTargetRadius(radius);
	arriveBehavior->SetSlowRadius(5.0f);
	agent.SetAgentImpl(new SteeringAgentImpl(arriveBehavior, speed));

	bool arrived = false;
	while (!arrived)
	{
		arrived = (glm::length(target.position - character.position) < 0.5f);

		elapsedTime = std::chrono::system_clock::now() - start;
		agent.UpdateAgent(elapsedTime.count());
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " Direction: " << character.orientation << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << std::endl <<
			"*************************************************************************** \n" << std::endl;
	}

	std::cout << "*********************************START ALIGN*******************************\n" << std::endl;
	target.orientation = 3.0875f;
	auto alignBehavior = new SteeringAlignBehavior(&character, &target);
	alignBehavior->SetMaxAcceleration(3.0f);
	alignBehavior->SetMaxSpeed(3.0f);
	alignBehavior->SetTargetRadius(radius);
	alignBehavior->SetSlowRadius(1.5f);
	agent.SetAgentImpl(new SteeringAgentImpl(alignBehavior, speed));

	bool aligned = false;
	while (!aligned)
	{
		elapsedTime = std::chrono::system_clock::now() - start;
		agent.UpdateAgent(elapsedTime.count());
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"Direction: " << character.orientation << " ---------> TARGET: " <<
			"Direction: " << target.orientation << std::endl <<
			"*************************************************************************** \n" << std::endl;

		aligned = abs(target.orientation - character.orientation) < 0.15f;
	}

	std::cout << "*********************************START PURSUE*******************************\n" << std::endl;
	character.position = glm::vec2(0);
	agent.SetAgentImpl(new SteeringAgentImpl(new SteeringPursueBehavior(&character, &target, speed, 0.1f), speed));

	Agent targetAgent(new KinematicAgentImpl(new KinematicWanderBehavior(&target, speed-1.5f, 0.7f)));

	while (glm::length(target.position - character.position) > 0.05f * speed)
	{
		elapsedTime = std::chrono::system_clock::now() - start;
		agent.UpdateAgent(elapsedTime.count());
		targetAgent.UpdateAgent(elapsedTime.count());
		std::cout << "**********************************CHARACTER********************************" << std::endl <<
			"X: " << character.position.x << ", Y: " << character.position.y << " Direction: " << character.orientation << " ---------> TARGET: " <<
			"X: " << target.position.x << ", Y: " << target.position.y << " Direction: " << target.orientation << std::endl <<
			"*************************************************************************** \n" << std::endl;
	}

	int e = 0;
	std::cin >> e;
	return 0;
}