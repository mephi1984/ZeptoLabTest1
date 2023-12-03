#include "ShaderManager.h"



namespace ZL {

	ShaderResource::ShaderResource(const std::string& vertexCode, const std::string& fragmentCode)
	{
		const int CONST_INFOLOG_LENGTH = 256;

		char infoLog[CONST_INFOLOG_LENGTH];
		int infoLogLength;

		int vertexShaderCompiled;
		int fragmentShaderCompiled;
		int programLinked;

		GLuint vertexShader;
		GLuint fragmentShader;

		int vertexCodeLength = static_cast<int>(strlen(vertexCode.c_str()));
		int fragmentCodeLength = static_cast<int>(strlen(fragmentCode.c_str()));

		const char* vc = &vertexCode[0];
		const char* fc = &fragmentCode[0];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &(vc), &vertexCodeLength);

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &(fc), &fragmentCodeLength);

		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
		glGetShaderInfoLog(vertexShader, CONST_INFOLOG_LENGTH, &infoLogLength, infoLog);

		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);
		glGetShaderInfoLog(fragmentShader, CONST_INFOLOG_LENGTH, &infoLogLength, infoLog);

		
		if (!vertexShaderCompiled)
		{
			throw std::runtime_error("Failed to compile vertex shader code!");
		}
		
		if (!fragmentShaderCompiled)
		{
			throw std::runtime_error("Failed to compile fragment shader code!");
		}

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programLinked);
		glGetProgramInfoLog(shaderProgram, CONST_INFOLOG_LENGTH, &infoLogLength, infoLog);
		
		if (!programLinked)
		{
			shaderProgram = 0;
			throw std::runtime_error("Failed to link shader program!");
		}


		int dummySize;			//Dummy
		int dummyLen;			//Dummy
		GLenum dummyEnum;


		//================= Parsing all uniforms ================

		int activeUniforms;

		const int CONST_UNIFORM_NAME_LENGTH = 256;
		char uniformName[CONST_UNIFORM_NAME_LENGTH];

		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &activeUniforms);

		for (int i = 0; i < activeUniforms; i++)
		{
			glGetActiveUniform(shaderProgram, i, CONST_UNIFORM_NAME_LENGTH, &dummyLen, &dummySize, &dummyEnum, uniformName);
			
			uniformList[uniformName] = glGetUniformLocation(shaderProgram, uniformName);
		}

		//================= Parsing all attributes ================
		int activeAttribs;

		const int CONST_ATTRIB_NAME_LENGTH = 256;
		char attribName[CONST_ATTRIB_NAME_LENGTH];

		glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &activeAttribs);

		for (int i = 0; i < activeAttribs; i++)
		{
			glGetActiveAttrib(shaderProgram, i, CONST_ATTRIB_NAME_LENGTH, &dummyLen, &dummySize, &dummyEnum, attribName);
			attribList[attribName] = glGetAttribLocation(shaderProgram, attribName);
		}
	}

	ShaderResource::~ShaderResource()
	{
		if (shaderProgram != 0)
		{
			glDeleteProgram(shaderProgram);

			shaderProgram = 0;
		}
	}

	GLuint ShaderResource::getShaderProgram()
	{
		return shaderProgram;
	}


	void ShaderManager::AddShaderFromFiles(const std::string& shaderName, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName)
	{
		std::string vertexShader = readTextFile(vertexShaderFileName);

		std::string fragmentShader = readTextFile(fragmentShaderFileName);

		shaderResourceMap[shaderName] = std::make_shared<ShaderResource>(vertexShader, fragmentShader);
	}

	void ShaderManager::PushShader(const std::string& shaderName)
	{
		if (shaderStack.size() >= CONST_MAX_SHADER_STACK_SIZE)
		{
			throw std::runtime_error("Shader stack overflow!");
		}

		if (shaderResourceMap.find(shaderName) == shaderResourceMap.end())
		{
			throw std::runtime_error("Shader does not exist!");
		}

		shaderStack.push(shaderName);

		glUseProgram(shaderResourceMap[shaderName]->getShaderProgram());
	}


	void ShaderManager::PopShader()
	{
		if (shaderStack.size() == 0)
		{
			throw std::runtime_error("Shader stack underflow!");
		}

		shaderStack.pop();

		if (shaderStack.size() == 0)
		{
			glUseProgram(0);
		}
		else
		{
			glUseProgram(shaderResourceMap[shaderStack.top()]->getShaderProgram());
		}
	}

	std::shared_ptr<ShaderResource> ShaderManager::GetCurrentShader()
	{
		if (shaderStack.size() == 0)
		{
			throw std::runtime_error("Shader stack underflow!");
		}


		return shaderResourceMap[shaderStack.top()];

	}




	ShaderSetter::ShaderSetter(ShaderManager& inShaderManager, const std::string& shaderName)
		: shaderManager(shaderManager)
	{
		shaderManager.PushShader(shaderName);
	}
	
	ShaderSetter::~ShaderSetter()
	{
		shaderManager.PopShader();
	}

}