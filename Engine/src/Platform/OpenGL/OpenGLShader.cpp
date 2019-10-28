#include "enginepch.h"

#include "OpenGLShader.h"

#include "glad/glad.h"

#include <fstream>
namespace Engine {

	static GLenum ShaderTypeFromString(const std::string &type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		ENGINE_ASSERT(false, "UNKNOWN SHADER TYPE!");
	}

	OpenGLShader::OpenGLShader(const std::string &filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0: lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string &name, std::string & vertexSrc, const std::string & fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string & name, int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string & name, float value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string & name, const glm::vec2 & value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string & name, const glm::vec3 & value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string & name, const glm::vec4 & value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string & name, const glm::mat3 & matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 & matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string & filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			ENGINE_LOG_ERROR("COULD NOT OPEN FILE '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string & source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char *typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t lineEnding = source.find_first_of("\r\n", pos);
			ENGINE_ASSERT(lineEnding != std::string::npos, "SYNTAX ERROR");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, lineEnding - begin);
			ENGINE_ASSERT(ShaderTypeFromString(type), "INVALID SHADER TYPE SPECIFIER");

			size_t nextLinePosition = source.find_first_not_of("\r\n", lineEnding);
			pos = source.find(typeToken, nextLinePosition);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePosition,
				(pos - (nextLinePosition == std::string::npos ? source.size() - 1 : nextLinePosition)));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSource)
	{
		ENGINE_ASSERT(shaderSource.size() <= 2, "CAN BE ONLY 2 SHADERS FOR NOW");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		m_RendererID = glCreateProgram();
		for (auto &kv : shaderSource)
		{
			GLenum type = kv.first;
			const std::string &source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar *sourceString = source.c_str();

			glShaderSource(shader, 1, &sourceString, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				ENGINE_LOG_ERROR("{0}", infoLog.data());
				ENGINE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(m_RendererID, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(m_RendererID);


		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);


			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RendererID);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			ENGINE_LOG_ERROR("{0}", infoLog.data());
			ENGINE_ASSERT(false, "OpenGLShader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(id, m_RendererID);
	}

}