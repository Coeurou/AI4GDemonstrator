#include "SteeringWanderBehavior.h"
#include "SpatialStructureUtility.h"

std::random_device SteeringWanderBehavior::rd;
std::mt19937 SteeringWanderBehavior::engine(rd());
std::uniform_real_distribution<float> SteeringWanderBehavior::distr(-1, 1);

SteeringWanderBehavior::SteeringWanderBehavior(SpatialStructure* character, float acc, float radius, float offset, float rate)

	: SteeringFaceBehavior(character, nullptr), wanderOrientation(0.0f)
{
	auto& parameters = GetParametersByRef();
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, acc };
	parameters[DISTANCE_PARAM] = { Parameter::FLOAT, offset };
	parameters[TURNRATE_PARAM] = { Parameter::FLOAT, rate };
	parameters[TARGETRADIUS_PARAM] = { Parameter::FLOAT, radius };
}

SteeringWanderBehavior::~SteeringWanderBehavior()
{}

KinematicSteeringOutput SteeringWanderBehavior::ComputeMovement()
{
	auto& parameters = GetParametersByRef();
	auto wanderRadius = parameters[TARGETRADIUS_PARAM].floatValue;
	auto wanderRate = parameters[TURNRATE_PARAM].floatValue;
	auto wanderOffset = parameters[DISTANCE_PARAM].floatValue;
	auto maxAcceleration = parameters[MAXACCELERATION_PARAM].floatValue;

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