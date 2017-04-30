#include "initializer.h"


initializer::initializer()
{
}

bool initializer::initAll()
{
	bool success = true;
	//init SDL
	if (initSDL())
	{
		cout << "[INFO]: SDL was initialized successfully" << endl;
	}
	else
	{
		cout << "[ERROR]: SDL wasn't initialised" << endl;
		success = false;
	}
	//initialise the event controller
	if (initEventCont())
	{
		cout << "[INFO]: Event Controller was initialized correctly" << endl;
	}
	else
	{
		cout << "[ERROR]: Event Controller wasn't initialized correctly" << endl;
		success = false;
	}
	//initialize the net controller
	if (initNetwork())
	{
		cout << "[INFO]: Net Controller was initialized correctly" << endl;
	}
	else
	{
		cout << "[ERROR]: Net Controller wasn't initialized correctly" << endl;
		success = false;
	}
	return success;
}

bool initializer::initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "[ERROR]: SDL failed to init. SDL_image Error: " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		cout << "[INFO]: SDL initalised." << endl;
		return true;
	}
}

bool initializer::initEventCont()
{
	return eventController::init();
}

bool initializer::initNetwork()
{
	return netController::init();
}

initializer::~initializer()
{
}
