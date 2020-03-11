#include <crtdbg.h>
#include "Game.h"

int main()
{
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game g(1600, 900);

	g.start();
	
	g.output_info("Test.txt");

	return 0;
}

