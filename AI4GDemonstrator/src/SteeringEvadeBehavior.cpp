#include "SteeringEvadeBehavior.h"

SteeringEvadeBehavior::SteeringEvadeBehavior(SpatialStructure* character, SpatialStructure* target, float speed, float prediction)
	: SteeringFleeBehavior(character, target, speed), maxPrediction(prediction)
{}

SteeringEvadeBehavior::~SteeringEvadeBehavior()
{}

KinematicSteeringOutput SteeringEvadeBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();
	auto target = GetTargetPtr();

	auto direction = target->position - character->position;
	auto distance = glm::length(direction);
	auto speed = glm::length(character->velocity);

	float prediction = (speed < distance / maxPrediction) ? maxPrediction : distance / speed;

	target->position += target->velocity * prediction;

	return SteeringFleeBehavior::ComputeMovement();
}