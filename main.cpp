#include <iostream>
#include "Src/Game.h"
#include "Src/Constants.h"

int main(int argc, char *args[]) {
	Game* game = new Game();

	// Initialize the Game 
	if (!game->Initialize()) return 1;

	game->Start();

	// Game Loop
	while (game->IsRunning()) {
		game->Update();
		game->HandleInput();
		game->Render();
	}

	return 0;
}