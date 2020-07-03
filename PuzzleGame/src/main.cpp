#include <Engine.h>

class Game : public Engine::Application
{
public:
	Game() {}
	~Game() {}

	void OnShutdown() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void OnUpdate(double& dt) override
	{
		TRACE("I am running on Update loop!");
	}

private:
};

Engine::Application* Engine::CreateApplication()
{
	return new Game();
}