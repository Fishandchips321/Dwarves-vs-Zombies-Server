#include "game.h"

game::game()
{
}

bool game::init()
{
	initializer initOb;
	if (!initOb.initAll())
	{
		return false;
	}
	myGameState = gameStates::lobby;
	FPSman = new FPSmanager;
	SDL_initFramerate(FPSman);
	SDL_setFramerate(FPSman, 60);

	return true;
}

bool game::startGameLoop()
{
	bool quit = false;
	std::cout << "[INFO]: Entering the game loop" << std::endl;

	while (!quit && myGameState != gameStates::stop)
	{
		while (eventController::nextEvent()) //poll events
		{
			if (eventController::currentEvent.type == SDL_QUIT) //if the window X button was pressed
			{
				quit = true;
			}

			eventUpdate();
		}

		if (netController::hostsConnected)
		{

		}

		//update the game
		update();

		SDL_framerateDelay(FPSman);
	}

	return true;
}

void game::eventUpdate()
{
	switch (myGameState)
	{
	case gameStates::play:
		entityController::eventUpdate();
		AIController::eventUpdate();
		break;
	case gameStates::lobby:
		break;
	case gameStates::stop:
		break;
	case gameStates::over:
		break;
	}
}

void game::update()
{
	switch (myGameState)
	{
	case gameStates::play:
		entityController::update();
		AIController::update();
		world.update();
		break;
	case gameStates::lobby:
		break;
	case gameStates::stop:
		break;
	case gameStates::over:
		break;
	}
}

game::~game()
{
}
