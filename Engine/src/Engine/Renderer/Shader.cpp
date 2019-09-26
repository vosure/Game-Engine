#pragma once

#include "enginepch.h"

#include "Shader.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	Shader * Shader::Create(const std::string & filepath)
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
			return new OpenGLShader(filepath);
		}break;
		}
		ENGINE_ASSERT(false, "Unknown RendererAPI!")
			return nullptr;
	}


	Shader *Shader::Create(const std::string &vertexSrc, const std::string &fragmentSrc)
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
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}break;
		}
		ENGINE_ASSERT(false, "Unknown RendererAPI!")
			return nullptr;
	}

}
