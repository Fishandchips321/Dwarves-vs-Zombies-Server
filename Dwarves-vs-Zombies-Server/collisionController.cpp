#include "collisionController.h"

std::vector<SDL_Rect* > collisionController::rects;
std::vector<entity* > collisionController::entities;

collisionController::collisionController()
{
}

void collisionController::registerCollider(SDL_Rect *rect)
{
	rects.push_back(rect);
	entities.push_back(nullptr);
}

void collisionController::registerCollider(entity* ent)
{
	rects.push_back(&ent->entityRect);
	entities.push_back(ent);
}

collisionController::collisionPacket collisionController::testCollision(entity* ent)
{
	for (size_t x = 0; x < rects.size(); x++)
	{
		collisionPacket packet;

		packet = collided(ent, *rects[x]);

		if (packet.collided)
		{
			packet.collidedEntity = ent;
			packet.colliderRect = rects[x];
			packet.colliderEntity = entities[x];
			return packet;
		}
	}
	collisionPacket packet;
	return packet;
}

collisionController::collisionPacket collisionController::collided(entity* ent, SDL_Rect B)
{
	collisionPacket packet;

	SDL_Rect A = ent->entityRect;

	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	//Calculate the sides of rect B
	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		packet.collided = true;
		packet.side = side::top;
	}

	if (topA >= bottomB)
	{
		packet.collided = true;
		packet.side = side::bottom;
	}

	if (rightA <= leftB)
	{
		packet.collided = true;
		packet.side = side::left;
	}

	if (leftA >= rightB)
	{
		packet.collided = true;
		packet.side = side::right;
	}

	//If none of the sides from A are outside B
	return packet;
}

collisionController::~collisionController()
{
}
