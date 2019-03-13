#include "Application.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Log.h"

namespace Engine {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent event(1280, 720);
		ENGINE_LOG_TRACE(event);

		while (true);
	}
	Application * CreateApplication()
	{
		return nullptr;
	}
}
