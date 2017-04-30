#include "playerNetAI.h"



playerNetAI::playerNetAI(int netID)
	:ID(netID)
{
	client = netController::clients[ID];
}

void playerNetAI::update()
{
	//move the entity
	ent->entityRect.x += ent->xVel;
	ent->entityRect.y += ent->yVel;
	//set the entity's direction

	//vertical
	if (ent->yVel < 0) //negative values = going up
		ent->direction = entity::up;

	if (ent->yVel > 0) //positive values = going down
		ent->direction = entity::down;
	//horizontal
	if (ent->xVel < 0) //negative values = going left
		ent->direction = entity::left;

	if (ent->xVel > 0) //positive values = going right
		ent->direction = entity::right;

	netUpdate(); //not called by anything external
}

void playerNetAI::netUpdate()
{
	client->nextGameMessage();

	//if there was a message in the queue
	if (client->currentGameMessage.NCM > -1)
	{
		switch (client->currentGameMessage.NCM)
		{
		case netController::PLAYER_UPDATE:
			//update the player's x and y coordinates
			ent->entityRect.x = atoi(client->currentGameMessage.paramaters[0].c_str());
			ent->entityRect.y = atoi(client->currentGameMessage.paramaters[1].c_str());
			//update the player's x and y velocity
			ent->xVel = atoi(client->currentGameMessage.paramaters[2].c_str());
			ent->yVel = atoi(client->currentGameMessage.paramaters[3].c_str());

			if (client->currentGameMessage.paramaters.size() == 5) //if the player's flying state has changed
			{
				ent->flying = atoi(client->currentGameMessage.paramaters[4].c_str()); //update the entity's flying state
			}

			break;
		case netController::PLAYER_ATTACK:
			itemController::getItem(stoi(client->currentGameMessage.paramaters[0]))->attack(ent); //get the item used to attack and call it's attack function
			break;
		case netController::PLAYER_PLACE:
			break;
		case netController::PLAYER_BREAK:
			break;
		}
	}
}

void playerNetAI::eventUpdate()
{

}


playerNetAI::~playerNetAI()
{
}
