#pragma once
#include "entity.h"

class tile
{
public:

	struct tileMeta
	{
		int metadata;
	};

	tile();
	void update(int x, int y);
	void onContact(entity* ent);
	~tile();
};