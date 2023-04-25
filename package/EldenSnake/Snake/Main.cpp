#include <iostream>
#include <time.h>
#include "Game.h"
#include "Log.h"
#include "Snake.h"


int main()
{
	srand((unsigned int)time(0));

	Game* game=new(std::nothrow) Game(DIF_NORMAL);
	if (game == NULL)
		return -1;
	game->run();

	return 0;
}