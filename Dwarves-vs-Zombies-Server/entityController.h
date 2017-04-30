#pragma once
#include "entity.h"
#include <vector>
#include <algorithm>
#include "counter.h"

class entityController
{
public:
	entityController();
	static bool init();
	static void update();
	static void eventUpdate();
	static bool registerEntity(entity* newEnt);
	static bool deleteEntity(entity* ent);
	~entityController();
private:
	static void autoclave(); //removes any dead entities thet should have been deleted;
	static counter ACtimer;
	static std::vector<entity* > entities;
};

