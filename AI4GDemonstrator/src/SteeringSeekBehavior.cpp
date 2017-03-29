#include "SteeringSeekBehavior.h"
#include "SpatialStructureUtility.h"
#include <glm.hpp>

SteeringSeekBehavior::SteeringSeekBehavior(SpatialStructure* character, SpatialStructure* target, float speed)
	: SteeringMovementBehavior(character, target)
{
	auto& parameters = GetParametersByRef();
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, speed };
}

SteeringSeekBehavior::~SteeringSeekBehavior()
{}

KinematicSteeringOutput SteeringSeekBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();
	auto& parameters = GetParametersByRef();

	movement.linear = target->position - character->position;
	movement.linear = glm::normalize(movement.linear) * parameters[MAXACCELERATION_PARAM].floatValue;
	movement.angular = 0;

	return movement;
}