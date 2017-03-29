#include "SteeringFleeBehavior.h"
#include "SpatialStructureUtility.h"
#include "SteeringSeekBehavior.h"
#include <glm.hpp>

SteeringFleeBehavior::SteeringFleeBehavior(SpatialStructure* character, SpatialStructure* target, float speed)
	: SteeringMovementBehavior(character, target)
{
	auto& parameters = GetParametersByRef();
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, speed };
}

SteeringFleeBehavior::~SteeringFleeBehavior()
{}

KinematicSteeringOutput SteeringFleeBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();
	auto& parameters = GetParametersByRef();

	movement.linear = character->position - target->position;
	movement.linear = glm::normalize(movement.linear) * parameters[MAXACCELERATION_PARAM].floatValue;
	movement.angular = 0;

	return movement;
}