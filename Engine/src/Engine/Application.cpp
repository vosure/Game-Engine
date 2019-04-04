#include "enginepch.h"

#include "Application.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Log.h"

#include <GLFW/glfw3.h>

namespace Engine {

#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(OnWindowClose));
		ENGINE_LOG_INFO(event.ToString());
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0.5f, 0, 0.44f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &event) 
	{
		m_Running = false;
		return true;
	}
	Application * CreateApplication()
	{
		return nullptr;
	}
}
