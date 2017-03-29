#include "SteeringVelocityMatchingBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>

SteeringVelocityMatchingBehavior::SteeringVelocityMatchingBehavior(SpatialStructure* character, SpatialStructure* target, float speed, float time)
	: SteeringMovementBehavior(character, target)
{
	auto& parameters = GetParametersByRef();
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, speed };
	parameters[TIMETOTARGET_PARAM] = { Parameter::FLOAT, time };
}

SteeringVelocityMatchingBehavior::~SteeringVelocityMatchingBehavior()
{}

KinematicSteeringOutput SteeringVelocityMatchingBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();
	auto& parameters = GetParametersByRef();
	auto timeToTarget = parameters[TIMETOTARGET_PARAM].floatValue;
	auto maxAcceleration = parameters[MAXACCELERATION_PARAM].floatValue;

	movement.linear = target->velocity - character->velocity;
	movement.linear /= timeToTarget;
	if (glm::length(movement.linear) > maxAcceleration) {
		movement.linear = glm::normalize(movement.linear) * maxAcceleration;
	}
	movement.angular = 0;

	return movement;
}