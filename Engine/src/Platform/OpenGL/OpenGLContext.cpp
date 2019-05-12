#include "enginepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"
namespace Engine {
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle)
		:m_WindowHandle(windowHandle)
	{
		ENGINE_ASSERT(m_WindowHandle, "Window Handle is NULL!");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_ASSERT(status, "Failed to initialize GLAD");

		ENGINE_LOG_INFO("OpenGL Info:");
		ENGINE_LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		ENGINE_LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		ENGINE_LOG_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
