#include "eventController.h"

SDL_Event eventController::currentEvent;

eventController::eventController()
{

}

bool eventController::init()
{
	Uint32 myEventType = SDL_RegisterEvents(5);
	if (myEventType != ((Uint32)-1))
		return true;
	else
		return false;
}

bool eventController::nextEvent()
{
	if (SDL_PollEvent(&currentEvent) != 0)
		return true;
	return false;
}

void eventController::sendEvent(int event, void* data1, void* data2)
{
	SDL_Event* e; //event container
	SDL_memset(&e, 0, sizeof(e));//clear the memory of the event
	e->type = event;//set the event type to the event ID passed
	e->user.data1 = data1;//set the data1 pointer
	e->user.data2 = data2;//set the data2 pointer
	SDL_PushEvent(e);//push the event
}


eventController::~eventController()
{
}
