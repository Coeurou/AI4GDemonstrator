#include "SteeringWanderBehavior.h"
#include "SpatialStructureUtility.h"

std::random_device SteeringWanderBehavior::rd;
std::mt19937 SteeringWanderBehavior::engine(rd());
std::uniform_real_distribution<float> SteeringWanderBehavior::distr(-1, 1);

SteeringWanderBehavior::SteeringWanderBehavior(SpatialStructure* character, float acc, float radius, float offset, float rate)

	: SteeringFaceBehavior(character, nullptr), maxAcceleration(acc), wanderRadius(radius),
	  wanderOffset(offset), wanderRate(rate), wanderOrientation(0.0f)
{}

SteeringWanderBehavior::~SteeringWanderBehavior()
{}

KinematicSteeringOutput SteeringWanderBehavior::ComputeMovement()
{
	wanderOrientation += distr(engine) * wanderRate;

	auto character = GetCharacterPtr();
	SpatialStructure target;
	target.orientation = wanderOrientation + character->orientation;

	auto characterOrientationAsVector = AngleAsVector(character->orientation);
	auto circleCenter = characterOrientationAsVector * wanderOffset;
	auto circleTarget = wanderRadius * wanderOrientation;

	target.position = circleCenter + circleTarget;

	SetTarget(&target);
	auto movement = SteeringFaceBehavior::ComputeMovement();
	movement.linear = maxAcceleration * characterOrientationAsVector;

	return movement;
}