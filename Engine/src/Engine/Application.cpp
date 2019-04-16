#include "enginepch.h"

#include "Application.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Log.h"

#include <GLAD/glad.h>

#include "Core.h"

namespace Engine {



	Application *Application::s_Instance = nullptr;

	Application::Application()
	{
		ENGINE_ASSERT(!s_Instance, "Application already exists")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));
		ENGINE_LOG_TRACE(event.ToString());
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer * layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0.5f, 0, 0.44f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer *layer : m_LayerStack)
				layer->OnUpdate();

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
