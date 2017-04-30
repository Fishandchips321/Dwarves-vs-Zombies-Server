#pragma once
#include <vector>
#include "eventController.h"
#include "netController.h"
#include "tileController.h"
#include "blockController.h"
#include "eventController.h"

class worldController
{
public:
	worldController();
	bool loadMap(std::string map);
	bool loadMap(int map);
	void update();
	void controlNetUpdate();
	~worldController();

	struct placedTile
	{
		tile* tile;
		tile::tileMeta metadata;
		int x, y;
	};

	struct placedBlock
	{
		block* block;
		block::blockMeta metadata;
		int x, y;
	};

	std::vector<placedTile> tiles;
	std::vector<placedBlock> blocks;

private:
	void updateTiles();
	void updateBlocks();

	struct worldMetadata
	{
		std::string version;

	};

	enum maps
	{
		test
	};

	const int maxMaps = 1;

	const std::string mapPaths[1] = { "assets/maps/text.xml" };

	worldMetadata metadata;
};

