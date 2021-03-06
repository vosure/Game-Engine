#pragma once

#include "Core.h"


namespace Engine {

	class ENGINE_API Input
	{
	public:

		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressImpl(keycode); }

		inline static bool IsMousePressed(int button) { return s_Instance->IsMousePressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressImpl(int keycode) = 0;

		virtual bool IsMousePressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input *s_Instance;
	};
}
