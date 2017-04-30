#include "entity.h"


entity::entity()
{
	entityRect = { 0, 0, 0, 0 };
	flying = false;
	isDead = false;
}

void entity::update()
{
}

void entity::eventUpdate()
{
}

void entity::onCollide(entity* collider)
{
}

bool entity::damage(int amount, entity* damager)
{
	return false;
}

bool entity::push(int xVel, int yVel)
{
	return false;
}

void entity::unload()
{

}

entity::~entity()
{
}
