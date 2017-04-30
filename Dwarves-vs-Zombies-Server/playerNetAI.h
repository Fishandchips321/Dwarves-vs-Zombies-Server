#pragma once
#include "AIPackage.h"
#include "netController.h"
#include "itemController.h"

class playerNetAI :
	public AIPackage
{
public:
	playerNetAI(int netID);
	void update();
	void netUpdate();
	void eventUpdate();
	~playerNetAI();

private:
	int ID;
	netController::clientData* client;
};

