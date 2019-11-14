#pragma once

#include "Engine.h"

class Sandbox2D : public Engine::Layer
{
public :
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep timestep) override;
	virtual void OnEvent(Engine::Event &event) override;

	virtual void OnImGuiRender() override;
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Ref<Engine::VertexArray> m_SquareVA;
	Engine::Ref<Engine::Shader> m_FlatColorShader;

	struct ProfileResult
	{
		const char *Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	Engine::Ref<Engine::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.5f, 0.9f, 1.0f };
};