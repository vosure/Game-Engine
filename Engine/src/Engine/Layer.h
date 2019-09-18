#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Core/Timestep.h"

namespace Engine {

	class ENGINE_API Layer
	{
	public:
		Layer(const std::string &name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event &event) {}

		virtual void OnImGuiRender() {}

		inline const std::string GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}