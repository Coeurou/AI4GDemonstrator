#include "BlendedBehavior.h"

BlendedBehavior::BlendedBehavior(std::initializer_list<std::pair<SteeringMovementBehavior*, float>> behaviorsAndWeight, float speed, float rotation)
	: SteeringMovementBehavior(nullptr, nullptr), behaviorsAndWeight(behaviorsAndWeight), maxAcceleration(speed), maxRotation(rotation)
{
	assert(behaviorsAndWeight.size() > 0);
	SetCharacter(behaviorsAndWeight.begin()->first->GetCharacterPtr());
	SetTarget(behaviorsAndWeight.begin()->first->GetTargetPtr());

	auto& parameters = SteeringMovementBehavior::GetParametersByRef();
	parameters.clear();
	for (auto& behavior : behaviorsAndWeight) {
		for (auto& param : behavior.first->GetParametersByRef()) {
			parameters.insert(param);
		}
	}
}

BlendedBehavior::BlendedBehavior(const std::vector<std::pair<SteeringMovementBehavior*, float>>& behaviorsAndWeight, float speed, float rotation)
	: SteeringMovementBehavior(nullptr, nullptr), behaviorsAndWeight(behaviorsAndWeight), maxAcceleration(speed), maxRotation(rotation)
{
	assert(behaviorsAndWeight.size() > 0);
	SetCharacter(behaviorsAndWeight[0].first->GetCharacterPtr());
	SetTarget(behaviorsAndWeight[0].first->GetTargetPtr());

	auto& parameters = SteeringMovementBehavior::GetParametersByRef();
	parameters.clear();
	for (auto& behavior : behaviorsAndWeight) {
		for (auto& param : behavior.first->GetParametersByRef()) {
			parameters.insert(param);
		}
	}
}

BlendedBehavior::~BlendedBehavior()
{}

KinematicSteeringOutput BlendedBehavior::ComputeMovement()
{
	KinematicSteeringOutput movement { glm::vec2(0), 0 };
	for (auto& pair : behaviorsAndWeight) {
		for (auto& param : GetParametersByRef()) {
			pair.first->SetParameter(param.first, param.second);
		}

		auto tempMovement = pair.first->ComputeMovement();
		movement.linear += tempMovement.linear * pair.second;
		movement.angular += tempMovement.angular * pair.second;
	}
	movement.linear = glm::min(movement.linear, glm::normalize(movement.linear)*maxAcceleration);
	movement.angular = std::fminf(movement.angular, maxRotation);
	return movement;
}

void BlendedBehavior::AddBehaviorAndWeight(size_t index, SteeringMovementBehavior* behavior, float weight)
{
	if (index >= behaviorsAndWeight.size()) {
		behaviorsAndWeight.emplace_back(behavior, weight);
	} 
	else {
		behaviorsAndWeight[index] = std::pair<SteeringMovementBehavior*, float>(behavior, weight);
	}
}