#pragma once
#include <vector>
#include "tile.h"
class tileController
{
public:
	tileController();
	static tile* getTile(int tileType);
	static bool registerTile(tile* tile);
	~tileController();


private:
	static std::vector<tile* > tiles;
};

