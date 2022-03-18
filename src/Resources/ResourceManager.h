#pragma once

#include<string>
#include<memory>
#include<map>


namespace Renderer
{
	class ShaderProgram;
	class Texture;
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

private:
	std::string getShaderCode(const std::string& relativeFilePath);

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap _shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture>> TexturesMap;
	TexturesMap _textures;

	std::string _path;
};