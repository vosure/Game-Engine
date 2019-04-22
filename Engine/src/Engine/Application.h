#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"

#include "Engine/ImGui/ImGuiLayer.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);

		static inline Application &Get() { return *s_Instance; }
		inline Window &GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent &event);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;

		static Application *s_Instance;
	};

	Application *CreateApplication();

}
