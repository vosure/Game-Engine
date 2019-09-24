#include <Engine.h>

#include "ImGui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{

		m_VertexArray.reset(Engine::VertexArray::Create());

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		Engine::Ref<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));
		Engine::BufferLayout layout =
		{
			{Engine::ShaderDataType::Float3, "a_Position"},
			{Engine::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		Engine::Ref<Engine::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Engine::VertexArray::Create());
		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Engine::Ref<Engine::VertexBuffer> squareVB;
		squareVB.reset(Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Engine::ShaderDataType::Float3, "a_Position" },
			{ Engine::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine::Ref<Engine::IndexBuffer> squareIB;
		squareIB.reset(Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(position, 1.0);
			}
			
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
			
		)";

		m_Shader.reset(Engine::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				gl_Position =u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
	
			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Engine::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			out vec2 v_TexCoord;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position =u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
	
			uniform sampler2D u_Texture;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Engine::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Engine::Texture2D::Create("assets/textures/brickwall.jpg");

		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Engine::Timestep ts) override
	{
		ENGINE_LOG_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_A))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_D))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		Engine::RenderCommand::SetClearColor({ 0.1f, 0.01f, 0.3f, 1.0f });
		Engine::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Engine::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 position(x * 0.15f, y * 0.15f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * scale;

				Engine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Engine::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//Engine::Renderer::Submit(m_Shader, m_VertexArray);

		Engine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Color Settings");
		ImGui::ColorPicker3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Engine::Event &event) override
	{
	}

private:
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::VertexArray> m_VertexArray;

	Engine::Ref<Engine::Shader> m_FlatColorShader, m_TextureShader;
	Engine::Ref<Engine::VertexArray> m_SquareVA;

	Engine::Ref<Engine::Texture2D> m_Texture;

	Engine::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 5.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.5f, 0.9f };
};

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}