#pragma once
#include <SDL.h>
#include <iostream>
class eventController
{
public:
	eventController();
	static bool init();
	static bool nextEvent();
	static void sendEvent(int event, void* data1 = nullptr, void* data2 = nullptr);
	~eventController();

	static SDL_Event currentEvent;

	enum events
	{
		disconnectNet = 32768,
		UIEvent,
		quitGame,
		mapLoaded,
		mapLoadError,
		quickExit,
		loginError,
		authError,
		spawnPlayer
	};

	enum UIEvents
	{
		showQuickplayMenu,
		showServerMenu,
		showOptionsMenu,
		showLobby,
		showGameUI,
		lobbyPlayerUpdate
	};

	struct playerSpawn
	{
		int playerID, x, y, playerClass;
	};
};

