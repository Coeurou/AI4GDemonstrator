#pragma once
#include "Subject.h"

class IObserver
{
public:
					IObserver()									{}
	virtual			~IObserver()								{}

	virtual void	OnNotify(Notification notificationType,
							 SpatialStructure entity)			= 0;
};