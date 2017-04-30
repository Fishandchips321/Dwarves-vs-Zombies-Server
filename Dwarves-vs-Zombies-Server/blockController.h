#pragma once
#include<vector>
#include "block.h"
class blockController
{
public:
	blockController();
	static block* getBlock(int blockType);
	static bool registerBlock(block* block);
	~blockController();

private:
	static std::vector<block* > blocks;
};

