#include "tileController.h"

std::vector<tile* > tileController::tiles;

tileController::tileController()
{
}

tile * tileController::getTile(int tileType)
{
	size_t ID = tileType;
	if (ID < tiles.size() - 1)
		return tiles[tileType];
	return nullptr;
}

bool tileController::registerTile(tile * tile)
{
	for (size_t x = 0; x < tiles.size(); x++)
	{
		if (tiles[x] == tile)
			return false;
	}
	tiles.push_back(tile);
	return true;
}


tileController::~tileController()
{
}
