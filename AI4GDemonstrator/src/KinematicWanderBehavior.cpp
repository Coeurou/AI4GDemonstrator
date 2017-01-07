#include "KinematicWanderBehavior.h"
#include "SpatialStructureUtility.h"
#include <random>

KinematicWanderBehavior::KinematicWanderBehavior(SpatialStructure* character, float speed, float rotation)
	: KinematicMovementBehavior(character, nullptr), maxSpeed(speed), maxRotation(rotation)
{}

KinematicWanderBehavior::~KinematicWanderBehavior()
{}

KinematicSteeringOutput KinematicWanderBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;

	auto character = GetCharacterPtr();

	movement.linear = AngleAsVector(character->orientation) * maxSpeed;

	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real_distribution<float> distr(-1, 1);

	movement.angular = character->orientation + distr(engine) * maxRotation;

	return movement;
}