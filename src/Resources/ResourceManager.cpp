#include "ResourceManager.h"

#include "../Renderer/ShaderProgram.h"

#include<sstream>
#include<fstream>
#include<iostream>

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