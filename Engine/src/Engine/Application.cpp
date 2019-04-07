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
	}

	void Application::PushOverlay(Layer * layer)
	{
		m_LayerStack.PushOverlay(layer);
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
