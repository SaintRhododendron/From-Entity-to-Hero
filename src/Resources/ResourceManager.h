#pragma once

#include<string>
#include<memory>
#include<map>
#include<vector>
#define RM_BASIC_INITIAL_SUB_TEXTURE_NAME "default"


namespace Renderer
{
	class ShaderProgram;
	class Texture;
	class Sprite;
}

class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaderProgram(const std::string& shaderProgramName, const std::string vertexPath, const std::string fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderProgramName);
	std::shared_ptr<Renderer::Texture> loadTexture(const std::string& textureName, const std::string& relativeTexturePath);
	std::shared_ptr<Renderer::Texture> getTexture(const std::string& textureName);
	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, 
												 const std::string& textureName,
												 const std::string& shaderProgramName,
											   	 const unsigned int spriteWidth,
												 const unsigned int spriteHeight,
												 const std::string& initialSubTextureName = RM_BASIC_INITIAL_SUB_TEXTURE_NAME);
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::Texture> loadTextureAtlas(const std::string& textureAtlasName, 
														const std::string& relativeTextureAtlasPath, 
														const std::vector<std::string> subTexturesNames,
														const unsigned int subTextureWidth,
														const unsigned int subTextureHeight);

private:
	std::string getShaderCode(const std::string& relativeFilePath);

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap _shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture>> TexturesMap;
	TexturesMap _textures;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	SpritesMap _sprites;

	std::string _path;
};