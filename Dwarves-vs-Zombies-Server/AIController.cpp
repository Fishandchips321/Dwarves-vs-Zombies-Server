#include "AIController.h"

std::vector<AIPackage* > AIController::packages;

AIController::AIController()
{
}

void AIController::update()
{
	for (size_t x = 0; x < packages.size(); x++)
	{
		packages[x]->update();
	}
}

void AIController::eventUpdate()
{
	for (size_t x = 0; x < packages.size(); x++)
	{
		packages[x]->eventUpdate();
	}
}

void AIController::netUpdate()
{
	for (size_t x = 0; x < packages.size(); x++)
	{
		packages[x]->netUpdate();
	}
}

bool AIController::registerAI(AIPackage * package, entity * ent)
{
	package->ent = ent;
	packages.push_back(package);
	return true;
}


AIController::~AIController()
{
}
