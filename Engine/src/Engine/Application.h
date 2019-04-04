#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &e);

		bool OnWindowClose(WindowCloseEvent &event);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application *CreateApplication();

}
