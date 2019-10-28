#include "Sandbox2D.h"

#include "ImGui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Engine/Core/EntryPoint.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Engine::Timestep timestep)
{
	m_CameraController.OnUpdate(timestep);

	Engine::RenderCommand::SetClearColor({ 0.1f, 0.01f, 0.3f, 1.0f });
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, {0.6f, 0.1f, 0.2f, 1.0f});
	Engine::Renderer2D::DrawQuad({ 0.5f, 0.0f }, { 0.5f, 0.5f }, { 0.1f, 0.1f, 0.8f, 1.0f });

	Engine::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Engine::Event &event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Color Settings");
	ImGui::ColorPicker3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
