#include "CollisionAvoidanceBehavior.h"
#include <limits>

CollisionAvoidanceBehavior::CollisionAvoidanceBehavior(SpatialStructure* character, const std::vector<SpatialStructure*>& targets, float radius, float acceleration)
	: SteeringMovementBehavior(character, nullptr), targets(targets)
{
	auto& parameters = GetParametersByRef();
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, acceleration };
	parameters[ENTITYRADIUS_PARAM] = { Parameter::FLOAT, radius };
}

CollisionAvoidanceBehavior::~CollisionAvoidanceBehavior()
{}

KinematicSteeringOutput CollisionAvoidanceBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement;
	auto character = GetCharacterPtr();
	SpatialStructure* targetToAvoid = nullptr;

	float shortestTime = std::numeric_limits<float>::max();
	float firstMinSeparation = std::numeric_limits<float>::max();
	float firstDistance = std::numeric_limits<float>::max();
	auto firstRelativePos = character->position;
	auto firstRelativeVel = character->velocity;

	auto& parameters = GetParametersByRef();
	auto doubleEntityRadius = parameters[ENTITYRADIUS_PARAM].floatValue * 2.0f;
	auto maxAcceleration = parameters[MAXACCELERATION_PARAM].floatValue;

	// Looking for the next collision
	for (auto target : targets) {
		auto relativePos = target->position - character->position;
		auto relativeVel = target->velocity - character->velocity;
		auto relativeSpeed = glm::length(relativeVel);
		auto distance = glm::length(relativePos);

		// If this time is positive and the smallest among targets we will avoid this target
		float timeToCollision = glm::dot(relativePos, relativeVel) / relativeSpeed*relativeSpeed;

		auto minSeparation = distance - relativeSpeed * timeToCollision;

		// If no collision occurs, we continue
		if (minSeparation > doubleEntityRadius) {
			continue;
		}

		// Store information from our potential collision target
		if (timeToCollision > 0 && timeToCollision < shortestTime) {
			shortestTime = timeToCollision;
			firstMinSeparation = minSeparation;
			firstDistance = distance;
			firstRelativePos = relativePos;
			firstRelativeVel = relativeVel;
			targetToAvoid = target;
		}
	}

	movement.angular = 0;
	if (targetToAvoid == nullptr) {
		movement.linear = glm::vec2(0);
		return movement;
	}
	
	// Compute the evade movement, first verify if we already collide with the target
	if (firstMinSeparation <= 0 || firstDistance < doubleEntityRadius) {
		firstRelativePos = targetToAvoid->position - character->position;
	}
	else {
		firstRelativePos += firstRelativeVel * shortestTime;
	}

	movement.linear = glm::normalize(-firstRelativePos) * maxAcceleration;

	return movement;
}
