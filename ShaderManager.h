#pragma once

#include "OpenGlExtensions.h"

#include "Utils.h"

namespace ZL {


	constexpr size_t CONST_MAX_SHADER_STACK_SIZE = 16;

	class ShaderResource
	{
	protected:
		GLuint shaderProgram;

		std::unordered_map<std::string, GLuint> uniformList;

		//std::unordered_map<std::string, std::pair<bool, size_t>>  UniformList;
		std::map<std::string, GLuint>  attribList;

		
	public:

		GLuint getShaderProgram();

		ShaderResource(const std::string& vertexCode, const std::string& fragmentCode);
		~ShaderResource();

	public:
		friend class ShaderManager;
		friend class Renderer;
	};

	class ShaderManager {
	protected:
		std::unordered_map<std::string, std::shared_ptr<ShaderResource>> shaderResourceMap;

		std::stack<std::string> shaderStack;

	public:
		void AddShaderFromFiles(const std::string& shaderName, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);

		void PushShader(const std::string& shaderName);
		void PopShader();

		std::shared_ptr<ShaderResource> GetCurrentShader();
	};


	class ShaderSetter
	{
	protected:

		ShaderManager& shaderManager;

	public:
		ShaderSetter(ShaderManager& inShaderManager, const std::string& shaderName);

		~ShaderSetter();


	};


}