#include "SDL.h"
#include "Loop.h"
#include <sstream>

Loop *loop(nullptr);

std::stringstream temp;
std::string str;

int main(int argc, char *argv[])
{
	// Notre fenêtre

	const int fps(120);
	const int frameDelay(1000 / fps);

	int frameTime(0);
	int frameStart(0);

	int ticks(0);
	int lastTick(0);
	int nbrFrame(0);

	loop = new Loop();

	if (loop->Init("Optique Geometrique", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700) < 0)
		return -1;

	// Boucle principale
	while ( Loop::isRunning)
	{
		loop->UpDate();
		loop->ManageEvent();
		loop->Render();

		ticks = SDL_GetTicks();
		frameStart = ticks - frameTime;
	
		if (frameDelay > frameStart)
			SDL_Delay(frameDelay - frameStart);

		frameTime = ticks;
		ticks = SDL_GetTicks();
		
		nbrFrame++;

		if (1000 < ticks - lastTick)
		{
			temp.str("");
			temp << nbrFrame;
			str = temp.str();
			Loop::fps = str.c_str();

			nbrFrame = 0;
			lastTick = ticks;
		}
		
		frameTime = ticks;
	}
	loop->Clean();

	delete loop;

	return 0; 
}