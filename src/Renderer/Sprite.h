#pragma once

#include <glad/glad.h>
#include <memory>
#include <glm/vec2.hpp>
#include <string>




namespace Renderer
{
	class ShaderProgram;
	class Texture;

	class Sprite
	{
	public:
		Sprite(const std::shared_ptr<Texture> pTexture,
			   const std::string initialSubTextureName,
			   const std::shared_ptr<ShaderProgram> pShaderProgram,
			   const glm::vec2& position = glm::vec2(0.f),
			   const glm::vec2& size = glm::vec2(1.f),
			   const float rotation = 0.0f);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void render();
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float rotation);
		glm::vec2 getSize() { return _size; }

	private:

		std::shared_ptr<Texture> _pTexture;
		std::shared_ptr<ShaderProgram> _pShaderProgram;
		glm::vec2 _position;
		glm::vec2 _size;
		float _rotation;

		GLuint _VAO;
		GLuint _vertexCoordsVBO;
		GLuint _textureCoordsVBO;
	};
}