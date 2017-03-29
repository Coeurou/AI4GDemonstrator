#pragma once

class IPrototype
{
public:
	IPrototype() {}
	virtual ~IPrototype() {}

	virtual IPrototype* Clone() = 0;
};