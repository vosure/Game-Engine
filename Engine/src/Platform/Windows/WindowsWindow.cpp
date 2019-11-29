#include "enginepch.h"
#include "WindowsWindow.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLAD/glad.h>

namespace Engine {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char *description) 
	{
		ENGINE_LOG_ERROR("GLFW Error ({0} : {1})", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps &props)
	{
		ENGINE_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		ENGINE_PROFILE_FUNCTION();

		ShutDown();
	}

	Window *Window::Create(const WindowProps &props)
	{
		return new WindowsWindow(props);
	}

	void WindowsWindow::Init(const WindowProps &props)
	{
		ENGINE_PROFILE_FUNCTION();	

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ENGINE_LOG_INFO("Creating Window {0}, {1} {2}", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			ENGINE_ASSERT(success, "Could not initialized GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
			}break;

			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
			}break;

			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
			}break;
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
			}break;

			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
			}break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(xOffset, yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(xPos, yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::ShutDown()
	{	
		ENGINE_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
			glfwTerminate();
	}

	void WindowsWindow::OnUpdate()
	{
		ENGINE_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		ENGINE_PROFILE_FUNCTION();

		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSyncEnabled() const
	{
		return m_Data.VSync;
	}

}