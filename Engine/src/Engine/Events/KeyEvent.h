#pragma once

#include "Event.h"

#include <sstream>

namespace Engine {

	class ENGINE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class ENGINE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeateCount) : KeyEvent(keycode), m_RepeateCount(repeateCount) {}

		inline int GetRepeatecount() const { return m_RepeateCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeateCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeateCount;
	};

}