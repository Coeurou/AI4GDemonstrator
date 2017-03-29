#include "WallAvoidanceBehavior.h"
#include "Collision.h"
#include <limits>

WallAvoidanceBehavior::WallAvoidanceBehavior(SpatialStructure* character, const std::vector<AABB>& targets, float speed, float evadeDistance, float rayLength)
	: SteeringSeekBehavior(character, nullptr, speed), targets(targets)
{
	auto& parameters = GetParametersByRef();
	parameters[DISTANCE_PARAM] = { Parameter::FLOAT, evadeDistance };
	parameters[RAYLENGTH_PARAM] = { Parameter::FLOAT, rayLength };
}

WallAvoidanceBehavior::~WallAvoidanceBehavior()
{}

KinematicSteeringOutput WallAvoidanceBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement{ glm::vec2(0), 0 };
	auto character = GetCharacterPtr();
	SpatialStructure* targetToAvoid = nullptr;

	auto& parameters = GetParametersByRef();
	auto avoidDistance = parameters[DISTANCE_PARAM].floatValue;
	auto rayLength = parameters[RAYLENGTH_PARAM].floatValue;

	auto ray = glm::normalize(character->velocity) * rayLength;
	auto collision = collisionDetector.GetCollision(targets, character->position, ray);
	if (glm::length(collision.normal) < 0.1f) {
		return movement;
	}
	else {
		SpatialStructure target;
		target.position = collision.position + collision.normal * avoidDistance;
	}

	return SteeringSeekBehavior::ComputeMovement();
}
