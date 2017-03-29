#include "SteeringPursueBehavior.h"

SteeringPursueBehavior::SteeringPursueBehavior(SpatialStructure* character, SpatialStructure* target, float speed, float prediction)
	: SteeringSeekBehavior(character, target, speed)
{
	auto& parameters = GetParametersByRef();
	parameters[MAXPREDICTION_PARAM] = { Parameter::FLOAT, prediction };
}

SteeringPursueBehavior::~SteeringPursueBehavior()
{}

KinematicSteeringOutput SteeringPursueBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	auto direction = target->position - character->position;
	auto distance = glm::length(direction);
	auto speed = glm::length(character->velocity);

	auto& parameters = GetParametersByRef();
	auto maxPrediction = parameters[MAXPREDICTION_PARAM].floatValue;
	float prediction = (speed < distance / maxPrediction) ? maxPrediction : distance / speed;

	auto tempPosition = target->position;
	target->position += target->velocity * prediction;

	movement = SteeringSeekBehavior::ComputeMovement();

	target->position = tempPosition;

	return movement;
}