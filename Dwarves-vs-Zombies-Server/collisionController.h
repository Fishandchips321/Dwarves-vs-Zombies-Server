#pragma once
#include <SDL.h>
#include <vector>
#include "entity.h"
class collisionController
{
public:
	enum side
	{
		none,
		top,
		bottom,
		left,
		right
	};

	struct collisionPacket
	{
		bool collided = false;
		SDL_Rect* colliderRect = nullptr;
		entity* collidedEntity = nullptr;
		side side = side::none;
		entity* colliderEntity = nullptr;
	};

	collisionController();
	static void registerCollider(SDL_Rect *rect);
	static void registerCollider(entity* ent);
	static collisionPacket testCollision(entity* ent);
	~collisionController();

private:
	static collisionPacket collided(entity* ent, SDL_Rect B);
	static std::vector<SDL_Rect* > rects;
	static std::vector<entity* > entities;
};

