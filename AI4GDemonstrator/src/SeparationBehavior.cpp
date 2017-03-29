#include "SeparationBehavior.h"

SeparationBehavior::SeparationBehavior(SpatialStructure* character, std::initializer_list<SpatialStructure*> targets, float minDistance, float decay, float maxAcc)
	: SteeringMovementBehavior(character, nullptr), targets(targets)
{
	auto& parameters = GetParametersByRef();
	parameters[DISTANCE_PARAM] = { Parameter::FLOAT, minDistance };
	parameters[DECAY_PARAM] = { Parameter::FLOAT, decay };
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, maxAcc };
}

SeparationBehavior::SeparationBehavior(SpatialStructure* character, const std::vector<SpatialStructure*>& targets, float minDistance, float decay, float maxAcc)
	: SteeringMovementBehavior(character, nullptr), targets(targets)
{
	auto& parameters = GetParametersByRef();
	parameters[DISTANCE_PARAM] = { Parameter::FLOAT, minDistance };
	parameters[DECAY_PARAM] = { Parameter::FLOAT, decay };
	parameters[MAXACCELERATION_PARAM] = { Parameter::FLOAT, maxAcc };
}

SeparationBehavior::~SeparationBehavior()
{}

KinematicSteeringOutput SeparationBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement{ glm::vec2(0), 0 };

	auto character = GetCharacterPtr();
	auto strength = 0.0f;

	auto& parameters = GetParametersByRef();
	auto minDistanceThreshold = parameters[DISTANCE_PARAM].floatValue;
	auto decayCoefficient = parameters[DECAY_PARAM].floatValue;
	auto maxAcceleration = parameters[MAXACCELERATION_PARAM].floatValue;

	// For loop multithread ?
	for (auto target : targets) {
		auto direction = character->position - target->position;
		float distance = glm::length(direction);
		if (distance < minDistanceThreshold) {
			// Separation intensity is function of distance to target squared and a decay value
			strength = std::fminf(decayCoefficient / distance*distance, maxAcceleration);
		}
		movement.linear += glm::normalize(direction) * strength;
	}

	return movement;
}
