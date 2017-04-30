#pragma once
#include <iostream>
#include <SDL.h>
#include <string>
#include "counter.h"
#include "eventController.h"
#include "netController.h"

using namespace std;

class initializer
{
public:
	initializer();
	bool initAll();
	bool initSDL();
	bool initEventCont();
	bool initNetwork();
	~initializer();
private:
};

