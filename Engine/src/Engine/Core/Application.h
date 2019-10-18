#pragma once

#include "Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"

#include "Engine/ImGui/ImGuiLayer.h"

#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"

#include "Engine/Renderer/OrthographicCamera.h"

#include "Engine/Core/Timestep.h"


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
		bool OnWindowResize(WindowResizeEvent &event);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;


		static Application *s_Instance;
	};

	Application *CreateApplication();

}
