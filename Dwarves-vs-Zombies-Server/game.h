#pragma once
#include <boost/thread.hpp>
#include <SDL2_framerate.h>
#include <iostream>
#include "netController.h"
#include "entityController.h"
#include "initializer.h"
#include "AIController.h"
#include "worldController.h"


class game
{
public:
	game();
	bool startGameLoop();
	bool init();
	~game();
private:

	enum class gameStates
	{
		lobby,
		play,
		over,
		stop,
		load
	};

	gameStates myGameState;

	void update();
	void eventUpdate();

	worldController world;

	FPSmanager* FPSman;
};

