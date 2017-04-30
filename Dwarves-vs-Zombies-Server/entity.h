#pragma once
#include <SDL.h>

class entity
{
	friend class AIPackage;
public:
	entity();
	virtual void update();
	virtual void eventUpdate();
	virtual void onCollide(entity* collider);
	virtual bool damage(int amount, entity* damager);
	virtual bool push(int xVel, int yVel);
	virtual void unload();

	SDL_Rect entityRect;
	int xVel, yVel;
	int speed;
	int direction;
	bool flying;
	bool isDead;

	enum direction
	{
		up,
		down,
		left,
		right
	};

	~entity();
};

