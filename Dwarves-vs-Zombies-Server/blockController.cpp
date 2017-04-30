#include "blockController.h"

std::vector<block* > blockController::blocks;

blockController::blockController()
{
}

block * blockController::getBlock(int blockType)
{
	size_t ID = blockType;
	if (ID < blocks.size() - 1)
		return blocks[blockType];

	return nullptr;
}

bool blockController::registerBlock(block * block)
{
	for (size_t x = 0; x < blocks.size(); x++)
	{
		if (blocks[x] == block)
			return false;
	}

	blocks.push_back(block);
	return true;
}


blockController::~blockController()
{
}
