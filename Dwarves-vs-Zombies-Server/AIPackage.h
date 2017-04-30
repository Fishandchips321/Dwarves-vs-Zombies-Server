#pragma once
#include "entity.h"
class AIPackage
{
public:
	AIPackage();
	virtual void update();
	virtual void eventUpdate();
	virtual void netUpdate();
	~AIPackage();

	entity* ent;

};

