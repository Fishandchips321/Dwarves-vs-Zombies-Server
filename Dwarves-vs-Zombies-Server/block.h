#pragma once
#include "entity.h"

class block
{
public:
	struct blockMeta
	{
		unsigned int metadata;
		int damage;
		int placerID;
	};

	block();
	void update(int x, int y);
	void draw(int x, int y);
	void onContact(entity* ent);
	~block();
};