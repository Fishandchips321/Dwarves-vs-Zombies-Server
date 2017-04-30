#include "entityController.h"

std::vector<entity* > entityController::entities;
counter entityController::ACtimer;

entityController::entityController()
{
}

bool entityController::init()
{
	ACtimer.start();
	return true;
}

void entityController::update()
{
	//update every registered entity
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->update();
	}
	//every 5 seconds, chsck for any dead entities and delete them
	if (ACtimer.getTicks() >= 5000)
	{
		autoclave();
		ACtimer.reset();
	}
}

void entityController::eventUpdate()
{
	//event update every registered entity
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->eventUpdate();
	}
}

bool entityController::registerEntity(entity* newEnt)
{

	if (newEnt != nullptr) //if the pointer passed is not null
	{
		entities.push_back(newEnt); //add the entity to the vector

		return true;
	}

	return false;
}

bool entityController::deleteEntity(entity * ent)
{
	//search for the entity remove it
	for(size_t x = 0; x < entities.size(); x++)
	{
		if (entities[x] == ent) //if the entity was found
		{
			delete entities[x]; //delete the object
			entities.erase(entities.begin() + x); //remove the pointer
			return true;
		}
	}
	return false; //the provided entity couldn't be found
}

entityController::~entityController()
{
	//delete and remove all entities
	for (size_t x = 0; x < entities.size(); x++)
	{
		delete entities[x];
		entities.erase(entities.begin() + 1);
	}
}

void entityController::autoclave()
{
	//search for dead entities and remove them
	for (size_t x = 0; x < entities.size(); x++)
	{
		if (entities[x]->isDead)
		{
			delete entities[x];
			entities.erase(entities.begin() + x);
		}
	}
}
