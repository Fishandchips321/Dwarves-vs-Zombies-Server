#pragma once
#include "entityController.h"
#include "AIController.h"
class spawnController
{
public:
	spawnController();
	static bool spawnEntity(int entityID, int AIID, int netID = -1); //spawns an entity of type entityID and with an AI package of type AIID. If the AI is the player net AI, the player's net ID must be supplied
	static bool spawnEntity(int entityID, AIPackage* package); //spawns an entity of type entityID with an already created AI package
	~spawnController();

	enum entities //fill with entity types
	{
		none
	};

	enum AIs //fill with AI types
	{
		NET_PLAYER
	};
};

