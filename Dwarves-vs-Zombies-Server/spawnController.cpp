#include "spawnController.h"
#include "AIPackageIncludes.h"
#include "entityIncludes.h"

spawnController::spawnController()
{
}

bool spawnController::spawnEntity(int entityID, int netID, int AIID)
{
	entity* ent;
	AIPackage *AI;

	switch (entityID)
	{
	default:
		return false;
	}

	switch (AIID)
	{
	case NET_PLAYER:
		if (netID >= 0)
		{
			playerNetAI* netAI = new playerNetAI(netID);
			AI = netAI;
		}
		break;
	default:
		return false;
	}

	if (!entityController::registerEntity(ent))
		return false;
	if (!AIController::registerAI(AI, ent))
		return false;

	return true;
}

bool spawnController::spawnEntity(int entityID, AIPackage * package)
{
	entity* ent;

	switch (entityID)
	{
	default:
		return false;
	}

	if (!entityController::registerEntity(ent))
		return false;
	package->ent = ent;

	return true;
}


spawnController::~spawnController()
{
}
