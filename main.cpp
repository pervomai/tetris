#include "Tetris.h"

int main(int argc, char* argv[])
{
	Tetris* tetris = new Tetris;
	srand((int)time(0));
	if (tetris->init())
	{
		while (tetris->menu())
		{
			while (tetris->isRunning())
			{
				tetris->setCurrentTime(SDL_GetTicks());
				tetris->handleEvents();
				tetris->gameplay();
				tetris->checkLine();
				tetris->checkGameOver();
				tetris->updateRenderer();
			}
		}
	}
	tetris->clean();
	tetris->quitSDL();
	return 0;
}