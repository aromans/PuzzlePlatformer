#pragma once

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();
	ENG_CORE_WARN("Welcome to the Engine!");
	INFO("Multi-Dimensional Puzzle Game ;)");

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}