#include <iostream>
#include <SDL.h>
#include "initializer.h"
#include "game.h"
#undef main

int main()
{
	game myGame;
	if (myGame.init())
	{
		myGame.startGameLoop();
	}
	else
	{
		std::cout << "[ERROR]: Game could not initialize. exiting..." << std::endl;
	}
	std::cout << "Bye!" << std::endl;
	return 0;
}