#pragma once
#include <vector>
#include "entity.h"
#include "AIPackage.h"
class AIController
{
public:
	AIController();
	static void update();
	static void eventUpdate();
	static void netUpdate();
	static bool registerAI(AIPackage* package, entity* ent);
	~AIController();

private:
	static std::vector<AIPackage* > packages;
};

