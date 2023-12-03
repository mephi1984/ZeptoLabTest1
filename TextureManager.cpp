#include "TextureManager.h"

namespace ZL
{

	Texture::Texture(const TextureDataStruct& texData)
	{

		width = texData.width;
		height = texData.height;

		glGenTextures(1, &texID);

		if (texID == 0)
		{
			throw std::exception("glGenTextures did not work");
		}

		glBindTexture(GL_TEXTURE_2D, texID);

		CheckGlError();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		CheckGlError();

		//This should be only for Windows
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		CheckGlError();

		if (texData.bitSize == TextureDataStruct::BS_24BIT)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(texData.width), static_cast<GLsizei>(texData.height), 0, GL_RGB, GL_UNSIGNED_BYTE, &texData.data[0]);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(texData.width), static_cast<GLsizei>(texData.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, &texData.data[0]);
		}

		CheckGlError();

	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texID);
		texID = 0;
	}

	GLuint Texture::getTexID()
	{
		return texID;
	}

	size_t Texture::getWidth()
	{
		return width;
	}

	size_t Texture::getHeight()
	{
		return height;
	}



	TextureDataStruct CreateTextureDataFromBmp24(const std::string& fullFileName)
	{

		TextureDataStruct texData;
		
		std::vector<char> fileArr = readFile(fullFileName);

		size_t fileSize = fileArr.size();

		if (fileSize < 22)
		{
			throw std::exception("File is too short or not correct!");
		}

		//This refers to BITMAPV5HEADER
		texData.width = *reinterpret_cast<uint32_t*>(&fileArr[18]);
		texData.height = *reinterpret_cast<uint32_t*>(&fileArr[22]);

		texData.bitSize = TextureDataStruct::BS_24BIT;

		size_t dataSize = texData.width * texData.height  * 3;

		texData.data.resize(dataSize);

		size_t pos = *reinterpret_cast<uint32_t*>(&fileArr[10]);
		size_t x = 0;

		for (size_t i = 0; i < texData.width; i++)
			for (size_t j = 0; j < texData.height; j++)
			{

				if (pos + 3 > fileSize)
				{
					throw std::exception("File is too short!");
				}
					

				x = (i * texData.height + j) + (i * texData.height + j) + (i * texData.height + j);

				texData.data[x + 2] = fileArr[pos++];
				texData.data[x + 1] = fileArr[pos++];
				texData.data[x + 0] = fileArr[pos++];

			}

		return texData;
	}

	TextureDataStruct CreateTextureDataFromBmp32(const std::string& fullFileName)
	{

		TextureDataStruct texData;

		std::vector<char> fileArr = readFile(fullFileName);

		size_t fileSize = fileArr.size();

		if (fileSize < 22)
		{
			throw std::exception("File is too short or not correct!");
		}

		//This refers to BITMAPV5HEADER
		texData.width = *reinterpret_cast<uint32_t*>(&fileArr[18]);
		texData.height = *reinterpret_cast<uint32_t*>(&fileArr[22]);

		texData.bitSize = TextureDataStruct::BS_32BIT;

		size_t dataSize = texData.width * texData.height * 4;

		texData.data.resize(dataSize);

		size_t pos = *reinterpret_cast<uint32_t*>(&fileArr[10]);
		size_t x = 0;

		for (size_t i = 0; i < texData.width; i++)
			for (size_t j = 0; j < texData.height; j++)
			{

				if (pos + 4 > fileSize)
				{
					throw std::exception("File is too short!");
				}


				x = (i * texData.height + j) + (i * texData.height + j) + (i * texData.height + j) + (i * texData.height + j);

				texData.data[x + 2] = fileArr[pos++];
				texData.data[x + 1] = fileArr[pos++];
				texData.data[x + 0] = fileArr[pos++];
				texData.data[x + 3] = fileArr[pos++];

			}

		return texData;
	}


}