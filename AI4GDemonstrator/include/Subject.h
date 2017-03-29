#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include "Notification.h"
#include "Observer.h"

class Subject
{
public:
							Subject()	{}
	virtual					~Subject()	{ Notify(Notification::SUBJECT_DISTROYED); }

	void Notify(Notification notificationType, SpatialStructure entity = SpatialStructure())
	{
		for (auto observer : observers) {
			observer->OnNotify(notificationType, entity);
		}
	}

	bool AddObserver(IObserver* observer)
	{
		if (std::find(observers.begin(), observers.end(), observer) != observers.end()) {
			std::cerr << "[Subject::AddObserver]Observer already present inside the collection." << std::endl;
			return false;
		}

		observers.emplace_back(observer);

		return true;
	}

	bool RemoveObserver(IObserver* observer)
	{
		bool res = false;
		auto toRemove = std::remove(observers.begin(), observers.end(), observer);
		
		if (toRemove != observers.end()) {
			observers.erase(toRemove);
			res = true;
		}

		return res;
	}

private:
	std::vector<IObserver*> observers;
};