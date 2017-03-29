#pragma once

#include "KinematicSteeringOutput.h"
#include "Parameter.h"
#include "SpatialStructure.h"
#include "Subject.h"
#include <map>
#include <string>


class SteeringMovementBehavior
{
public:
										SteeringMovementBehavior(SpatialStructure* character,
																 SpatialStructure* target) 
											: character(character), target(target)				{ if (character != nullptr) assert(character != target); }
	virtual								~SteeringMovementBehavior()								{}

	virtual KinematicSteeringOutput		ComputeMovement()										{ return KinematicSteeringOutput(); }

	SpatialStructure					GetCharacter() const									{ return *character; }
	SpatialStructure					GetTarget()	   const									{ return *target; }
	void								SetTarget(SpatialStructure* target)						{ this->target = target; }

	SpatialStructure*					GetCharacterPtr()										{ return character; }
	SpatialStructure*					GetTargetPtr()											{ return target; }

	virtual	std::string					ToString()												= 0;

	std::map<const char*, Parameter>	GetParameters() const									{ return parameters; }
	std::map<const char*, Parameter>&	GetParametersByRef()									{ return parameters; }

	bool								SetParameter(const char* paramName, Parameter param)
	{
		auto paramFound = parameters.find(paramName);
		bool res = paramFound != parameters.end();
		if (res) {
			paramFound->second = param;
		}

		return res;
	}

protected:
	void								SetCharacter(SpatialStructure* c)						{ character = c; }

private:
	SpatialStructure*					character;
	SpatialStructure*					target;
	std::map<const char*, Parameter>	parameters;
};