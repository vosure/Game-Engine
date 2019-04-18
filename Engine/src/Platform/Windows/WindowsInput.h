#pragma once

#include "Engine/Input.h"

namespace Engine {

	class WindowsInput : public Input
	{
		virtual bool IsKeyPressImpl(int keycode) override;

		virtual bool IsMousePressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}