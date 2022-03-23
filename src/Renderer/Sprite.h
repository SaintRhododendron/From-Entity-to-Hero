#pragma once

#include <glad/glad.h>
#include <memory>
#include <glm/vec2.hpp>
#include <string>
#define SP_BASIC_ROTATION 0.0f
#define SP_BASIC_POSITION glm::vec2(0.f)
#define SP_BASIC_SIZE glm::vec2(1.f)

#define SP_ROTATION_LEFT_RIGHT RotationStyle::leftRight
#define SP_ROTATION_ALL_AROUND RotationStyle::allAround
#define SP_ROTATION_DO_NOT_ROTATE RotationStyle::doNotRotate



namespace Renderer
{
	class ShaderProgram;
	class Texture;

	class Sprite
	{
	public:

		enum RotationStyle
		{
			allAround,
			leftRight,
			doNotRotate
		};

		Sprite(const std::shared_ptr<Texture> pTexture,
			   const std::string initialSubTextureName,
			   const std::shared_ptr<ShaderProgram> pShaderProgram,
			   const glm::vec2& position = SP_BASIC_POSITION,
			   const glm::vec2& size = SP_BASIC_SIZE,
			   const float rotation = SP_BASIC_ROTATION,
			   const RotationStyle rotationStyle = SP_ROTATION_ALL_AROUND);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void setSubTexture(const std::string subTextureName);

		void render();
		void setPosition(const glm::vec2& position);
		glm::vec2 getPosition() { return _position; }

		void setSize(const glm::vec2& size);
		glm::vec2 getSize() { return _size; }

		void setRotation(const float rotation);
		float getRotation() { return _rotation; }

		void setRotationStyle(RotationStyle rotationStyle);
		RotationStyle getRotationStyle() {	return _rotationStyle;	}

	protected:

		std::shared_ptr<Texture> _pTexture;
		std::shared_ptr<ShaderProgram> _pShaderProgram;
		glm::vec2 _position;
		glm::vec2 _size;
		float _rotation;

		GLuint _VAO;
		GLuint _vertexCoordsVBO;
		GLuint _textureCoordsVBO;

		RotationStyle _rotationStyle;
	};
}