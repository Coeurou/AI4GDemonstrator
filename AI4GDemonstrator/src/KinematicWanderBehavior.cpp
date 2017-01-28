#include "KinematicWanderBehavior.h"
#include "SpatialStructureUtility.h"

std::random_device KinematicWanderBehavior::rd;
std::mt19937 KinematicWanderBehavior::engine(rd());
std::uniform_real_distribution<float> KinematicWanderBehavior::distr(-1, 1);

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
	movement.angular = character->orientation + distr(engine) * maxRotation;

	return movement;
}