#include "SteeringArriveBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>

SteeringArriveBehavior::SteeringArriveBehavior(SpatialStructure* character, SpatialStructure* target)
	: SteeringMovementBehavior(character, target)
{
	auto& parameters = GetParametersByRef();
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, 1.0f };
	parameters[MAXSPEED_PARAM] = { Parameter::FLOAT, 1.0f };
	parameters[TARGETRADIUS_PARAM] = { Parameter::FLOAT, 0.2f };
	parameters[SLOWRADIUS_PARAM] = { Parameter::FLOAT, 0.75f };
	parameters[TIMETOTARGET_PARAM] = { Parameter::FLOAT, 0.1f };
}

SteeringArriveBehavior::~SteeringArriveBehavior()
{}

KinematicSteeringOutput SteeringArriveBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	auto direction = target->position - character->position;
	float distance = glm::length(direction);
	
	auto& parameters = GetParametersByRef();
	auto targetRadius = parameters[TARGETRADIUS_PARAM].floatValue;
	auto slowRadius = parameters[SLOWRADIUS_PARAM].floatValue;
	auto maxSpeed = parameters[MAXSPEED_PARAM].floatValue;
	auto timeToArrive = parameters[TIMETOTARGET_PARAM].floatValue;
	auto maxAcceleration = parameters[MAXACCELERATION_PARAM].floatValue;

	movement.angular = 0;
	if (distance < targetRadius) {
		movement.linear = glm::vec2(0);
		return movement;
	}

	float targetSpeed = (distance > slowRadius) ? maxSpeed : maxSpeed * distance / slowRadius;

	direction = glm::normalize(direction) * targetSpeed;
	movement.linear = direction - character->velocity;
	movement.linear /= timeToArrive;

	if (glm::length(movement.linear) > maxAcceleration) {
		movement.linear = glm::normalize(movement.linear) * maxAcceleration;
	}

	return movement;
}