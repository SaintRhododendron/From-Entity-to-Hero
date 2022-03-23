#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "stb_image.h"

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sprite.h"

#include<sstream>
#include<fstream>
#include<iostream>

#define RM_TEXTURE_FIX 0.01f

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t position = executablePath.find_last_of("/\\");
	_path = executablePath.substr(0, position);
}

std::string ResourceManager::getShaderCode(const std::string& relativeFilePath)
{
	std::ifstream file;
	file.open(_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "[ERR] Failed to open shader code: " << _path << "/" << relativeFilePath << std::endl;
		return std::string{};
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaderProgram(const std::string& shaderProgramName, 
																			const std::string vertexPath, 
																			const std::string fragmentPath)
{
	std::string vertexShaderCode = getShaderCode(vertexPath);
	if (vertexShaderCode.empty())
	{
		std::cerr << "[ERR] Vertex shader " << vertexPath << " is empty." << std::endl;
		return nullptr;
	}
	std::string fragmentShaderCode = getShaderCode(fragmentPath);
	if (fragmentShaderCode.empty())
	{
		std::cerr << "[ERR] Fragment shader " << fragmentPath << " is empty." << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::ShaderProgram> newShaderProgram = _shaderPrograms.emplace(shaderProgramName, std::make_shared<Renderer::ShaderProgram>(vertexShaderCode, fragmentShaderCode)).first->second;
	if (newShaderProgram->isCompiled())
	{
		return newShaderProgram;
	}
	std::cerr << "[ERR] Shader program " << shaderProgramName << " failed to compile!\n"
		<< "\tVertex: " << vertexPath << std::endl
		<< "\tFragment: " << fragmentPath << std::endl;
	return nullptr;

	
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderProgramName)
{
	ShaderProgramsMap::const_iterator iterator = _shaderPrograms.find(shaderProgramName);
	if (iterator != _shaderPrograms.end())
	{
		return iterator->second;
	}
	std::cerr << "[ERR] Failed to find Shader program " << shaderProgramName << " in shaderProgramsMap." << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator iterator = _textures.find(textureName);
	if (iterator != _textures.end())
	{
		return iterator->second;
	}
	std::cerr << "[ERR] Failed to find Texture " << textureName << " in TexturesMap." << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture> ResourceManager::loadTexture(const std::string& textureName, const std::string& relativeTexturePath)
{
	int channelsAmount = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(_path + "/" + relativeTexturePath).c_str(), &width, &height, &channelsAmount, 0);


	if (!pixels)
	{
		std::cerr << "[ERR] Failed to load Texture: " << relativeTexturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::Texture> pNewTexture = _textures.emplace(textureName, std::make_shared<Renderer::Texture>(width, height, pixels, channelsAmount, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);
	return pNewTexture;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator iterator = _sprites.find(spriteName);
	if (iterator != _sprites.end())
	{
		return iterator->second;
	}
	std::cerr << "[ERR] Failed to find Sprite " << spriteName << " in SpritesMap." << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
															  const std::string& textureName,
															  const std::string& shaderProgramName,
															  const unsigned int spriteWidth,
														 	  const unsigned int spriteHeight,
															  const std::string& initialSubTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "[ERR] Failed to get Texture " << textureName << " for Sprite " << spriteName << std::endl;
		return nullptr;
	}
	auto pShaderProgram = getShaderProgram(shaderProgramName);
	if (!pShaderProgram)
	{
		std::cerr << "[ERR] Failed to get Shader Program " << shaderProgramName << " for Sprite " << spriteName << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::Sprite> newSprite = _sprites.emplace(spriteName, std::make_shared<Renderer::Sprite>(pTexture, initialSubTextureName, pShaderProgram, glm::vec2(0.f), glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;
}

std::shared_ptr<Renderer::Texture> ResourceManager::loadTextureAtlas(const std::string& textureAtlasName,
																	 const std::string& relativeTextureAtlasPath,
																	 const std::vector<std::string> subTexturesNames,
																	 const unsigned int subTextureWidth,
																	 const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(textureAtlasName, relativeTextureAtlasPath);
	if (pTexture)
	{
		const float textureWidth = pTexture->getWidth();
		const float textureHeight = pTexture->getHeight();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;
		for (const auto& currentSubTextureName : subTexturesNames)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + RM_TEXTURE_FIX) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight + RM_TEXTURE_FIX) / textureHeight);
			glm::vec2 rightBottomUV(static_cast<float>(currentTextureOffsetX - RM_TEXTURE_FIX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY - RM_TEXTURE_FIX) / textureHeight);
			
			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}

			pTexture->addSubTexture(currentSubTextureName, leftBottomUV, rightBottomUV);
		}
	}
	return pTexture;
}