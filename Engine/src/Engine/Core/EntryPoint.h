#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application *Engine::CreateApplication();

int main(int argc, char **argv) 
{
	Engine::Log::Init();
	
	ENGINE_PROFILE_BEGIN_SESSION("Startup", "Profile - Startup.json");
	auto app = Engine::CreateApplication();
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Runtime", "Profile - Runtime.json");
	app->Run();
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Shutdown", "Profile - Shutdown.json");
	delete app;
	ENGINE_PROFILE_END_SESSION();
}

#endif 
