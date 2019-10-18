#include "enginepch.h"

#include "OrthorgraphicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

namespace Engine {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(ENGINE_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(ENGINE_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;

		if (m_Rotation)
		{

			if (Engine::Input::IsKeyPressed(ENGINE_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * timestep;
			if (Engine::Input::IsKeyPressed(ENGINE_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * timestep;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent & event)
	{
		m_ZoomLevel -= event.getYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent & event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

}