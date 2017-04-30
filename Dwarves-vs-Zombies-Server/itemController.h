#pragma once
#include <vector>
#include "item.h"

class itemController
{
public:
	itemController();
	static bool registerItem(item* newItem);
	static item* getItem(int itemID);
	~itemController();
private:
	static std::vector<item* > items;
};

