#include <Engine.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
	}

	void OnUpdate() override
	{
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Kulity");
		ImGui::End();
	}

	void OnEvent(Engine::Event &event) override
	{
		if (event.GetEventType() == Engine::EventType::KeyPressed)
		{
			Engine::KeyPressedEvent &keyPressedEvent = (Engine::KeyPressedEvent &)event;

			ENGINE_LOG_TRACE("{0}", (char)keyPressedEvent.GetKeyCode());
		}
	}
};

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}