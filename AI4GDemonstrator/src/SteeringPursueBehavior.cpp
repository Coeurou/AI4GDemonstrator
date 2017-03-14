#include "SteeringPursueBehavior.h"

SteeringPursueBehavior::SteeringPursueBehavior(SpatialStructure* character, SpatialStructure* target, float speed, float prediction)
	: SteeringSeekBehavior(character, target, speed), maxPrediction(prediction)
{}

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

	float prediction = (speed < distance / maxPrediction) ? maxPrediction : distance / speed;

	auto tempPosition = target->position;
	target->position += target->velocity * prediction;

	movement = SteeringSeekBehavior::ComputeMovement();

	target->position = tempPosition;

	return movement;
}