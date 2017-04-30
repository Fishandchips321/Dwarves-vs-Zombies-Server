#pragma once
#include <SDL.h>
class counter
{
public:
	counter();
	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();
	void reset();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

	~counter();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

