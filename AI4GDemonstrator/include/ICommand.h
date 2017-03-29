#pragma once
#include "SpatialStructure.h"

class ICommand
{
					ICommand()							{}
	virtual			~ICommand()							{}

	virtual void	Execute()							= 0;
	virtual void	Execute(SpatialStructure& entity)	{}
};