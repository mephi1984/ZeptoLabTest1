#pragma once

#include "OpenGlExtensions.h"

#include "Utils.h"

namespace ZL
{

	struct TextureDataStruct
	{
		size_t width;
		size_t height;
		std::vector<char> data;
		enum BitSize {
			BS_24BIT,
			BS_32BIT
		};

		BitSize bitSize;
	};

	class Texture
	{
		size_t width = 0;
		size_t height = 0;
		GLuint texID = 0;

	public:

		Texture(const TextureDataStruct& texData);

		~Texture();

		GLuint getTexID();

		size_t getWidth();
		size_t getHeight();


	};

	TextureDataStruct CreateTextureDataFromBmp24(const std::string& fullFileName);
	TextureDataStruct CreateTextureDataFromBmp32(const std::string& fullFileName);
}
