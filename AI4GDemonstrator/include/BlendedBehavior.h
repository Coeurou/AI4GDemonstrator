#pragma once
#include "SteeringMovementBehavior.h"
#include <vector>
#include <sstream>
#include <string>

class BlendedBehavior : public SteeringMovementBehavior
{
public:
																BlendedBehavior(std::initializer_list<std::pair<SteeringMovementBehavior*, float>> behaviorsAndWeight,
																				float speed, float rotation);
																BlendedBehavior(const std::vector<std::pair<SteeringMovementBehavior*, float>>& behaviorsAndWeight,
																				float speed, float rotation);
																~BlendedBehavior();

	KinematicSteeringOutput										ComputeMovement() override;
	void														AddBehaviorAndWeight(size_t index, SteeringMovementBehavior* behavior, float weight);

	std::string ToString() override 
	{
		std::ostringstream res;
		for (auto pairBehaviorVal : behaviorsAndWeight) {
			res << " - " << pairBehaviorVal.first->ToString();
		}
		return res.str();
	}

private:
	std::vector<std::pair<SteeringMovementBehavior*, float>>	behaviorsAndWeight;
	float														maxAcceleration;
	float														maxRotation;
};
