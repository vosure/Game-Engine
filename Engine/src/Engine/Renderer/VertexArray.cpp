#pragma once

#include "enginepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			ENGINE_ASSERT(false, "Not supported!");
			return nullptr;
		}break;
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLVertexArray>();
		}break;
		}
		ENGINE_ASSERT(false, "Unknown RendererAPI!")
			return nullptr;
	}

}
