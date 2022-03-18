#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator= (ShaderProgram&) = delete;
		ShaderProgram& operator= (ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

		bool isCompiled() { return _isCompiled; }
		void use();
		bool createShader(const std::string source, const GLenum shaderType, GLuint& shaderID);

	private:
		bool _isCompiled = false;
		GLuint _ID;
	};
}