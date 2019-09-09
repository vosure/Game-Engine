#pragma once

#include "enginepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine {

	VertexBuffer *VertexBuffer::Create(float * vertices, uint32_t size)
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
			return new OpenGLVertexBuffer(vertices, size);
		}break;
		}
		ENGINE_ASSERT(false, "Unknown RendererAPI!")
			return nullptr;
	}

	IndexBuffer * IndexBuffer::Create(uint32_t * indices, uint32_t size)
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
			return new OpenGLIndexBuffer(indices, size);
		}break;
		}
		ENGINE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

}