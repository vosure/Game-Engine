#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application *Engine::CreateApplication();

int main(int argc, char **argv) 
{
	Engine::Log::Init();
	
	ENGINE_LOG_WARN("Initialized Logger");
	CLIENT_LOG_INFO("HELLO THERE");

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif 
