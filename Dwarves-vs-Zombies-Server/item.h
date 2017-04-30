#pragma once
#include "entity.h"
class item
{
public:
	item();
	virtual bool attack(entity* attacker);
	virtual bool use(entity* user);
	~item();
};

