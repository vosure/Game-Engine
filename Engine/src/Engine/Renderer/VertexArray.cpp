#pragma once

#include "enginepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine {

	VertexArray *VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
		{
			ENGINE_ASSERT(false, "Not supported!");
			return nullptr;
		}break;
		case RendererAPI::OpenGL:
		{
			return new OpenGLVertexArray();
		}break;
		}
		ENGINE_ASSERT(false, "Unknown RendererAPI!")
			return nullptr;
	}

}
