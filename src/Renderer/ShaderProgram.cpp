#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

namespace Renderer
{
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint vertexShaderID = 0;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "[ERR] Can not compile Vertex Shader." << std::endl;
			return;
		}
		GLuint fragmentShaderID = 0;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr << "[ERR] Can not compile Fragment Shader." << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		_ID = glCreateProgram();
		glAttachShader(_ID, vertexShaderID);
		glAttachShader(_ID, fragmentShaderID);
		glLinkProgram(_ID);

		GLint success;
		glGetProgramiv(_ID, GL_LINK_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(_ID, 1024, nullptr, infoLog);
			std::cerr << "[ERR] ShaderProgram-linking error. " << infoLog << std::endl;
		}
		else
		{
			_isCompiled = true;
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram& ShaderProgram::operator= (ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(_ID);
		_ID = shaderProgram._ID;
		_isCompiled = shaderProgram._isCompiled;

		shaderProgram._ID = 0;
		shaderProgram._isCompiled = false;
		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		_ID = shaderProgram._ID;
		_isCompiled = shaderProgram._isCompiled;

		shaderProgram._ID = 0;
		shaderProgram._isCompiled = false;
	}


	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(_ID);
		_isCompiled = false;
	}

	void ShaderProgram::use()
	{
		glUseProgram(_ID);
	}

	bool ShaderProgram::createShader(const std::string source, const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::cerr << "[ERR] Shader-compiling error. " << infoLog << std::endl;
			return false;
		}
		return true;
	}

	void ShaderProgram::setInt(const std::string name, GLint value)
	{
		glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
	}

	void ShaderProgram::setMatrix4(const std::string name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}