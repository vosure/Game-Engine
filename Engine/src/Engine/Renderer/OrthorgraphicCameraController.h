#pragma once

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Engine {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event &event);

		const inline OrthographicCamera &GetCamera() const { return m_Camera; }
		inline OrthographicCamera &GetCamera() { return m_Camera; }

		const float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 50.0f;

		OrthographicCamera m_Camera;

	private:
		bool OnMouseScrolled(MouseScrolledEvent &event);
		bool OnWindowResized(WindowResizeEvent &event);
	};

}