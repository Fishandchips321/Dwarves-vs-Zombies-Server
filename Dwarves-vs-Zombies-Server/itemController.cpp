#include "itemController.h"

std::vector<item* > itemController::items;

itemController::itemController()
{
}

bool itemController::registerItem(item* newItem)
{
	if (newItem != nullptr)
	{
		items.push_back(newItem);
		return true;
	}
	return false;
}

item* itemController::getItem(int itemID)
{
	if ((size_t)itemID < items.size())
	{
		return items[itemID];
	}
	else
	{
		return nullptr;
	}
}

itemController::~itemController()
{
}
