#include "Sandbox2D.h"

#include "ImGui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Engine/Core/EntryPoint.h"

#include "Engine/Renderer/Renderer2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Engine::Texture2D::Create("assets/textures/roflan.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Engine::Timestep timestep)
{
	ENGINE_PROFILE_FUNCTION();

	{
		ENGINE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(timestep);
	}

	{
		ENGINE_PROFILE_SCOPE("Renderer Prep");
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Engine::RenderCommand::Clear();
	}

	{
		ENGINE_PROFILE_SCOPE("Renderer Draw");
		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Engine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Engine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Engine::Event &event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();

	ImGui::Begin("Color Settings");
	ImGui::ColorPicker3("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}
