#include <iostream>
#include "Game.h"
#include "Constants.h"

int main(int argc, char *args[]) {
	Engine::Game* game = new Engine::Game();

	// Initialize the Game 
	if (!game->Initialize()) return 1;

	game->Start();

	double t = 0.0;
	const double dt = 0.01;
	double currTime = glfwGetTime();
	double accumulator = 0.0;

	// Game Loop
	while (game->IsRunning()) {

		double newTime = glfwGetTime();
		double frameTime = newTime - currTime;
		if (frameTime > 0.25)
			frameTime = 0.25;
		currTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt) {
			game->HandleInput();
			game->Update(dt);
			t += dt;
			accumulator -= dt;
		}

		game->Render();
	}

	return 0;
}