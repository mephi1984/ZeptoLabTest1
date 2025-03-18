#include "TextureManager.h"
#ifdef PNG_ENABLED
#include "png.h"
#endif

namespace ZL
{

	Texture::Texture(const TextureDataStruct& texData)
	{

		width = texData.width;
		height = texData.height;

		glGenTextures(1, &texID);

		if (texID == 0)
		{
			throw std::runtime_error("glGenTextures did not work");
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



	TextureDataStruct CreateTextureDataFromBmp24(const std::string& fullFileName, const std::string& ZIPFileName)
	{

		TextureDataStruct texData;
		std::vector<char> fileArr;

		if (!ZIPFileName.empty()) {
			fileArr = readFileFromZIP(fullFileName, ZIPFileName);
		}
		else {
			fileArr = readFile(fullFileName);
		}

		size_t fileSize = fileArr.size();

		if (fileSize < 22)
		{
			throw std::runtime_error("File is too short or not correct!");
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
					throw std::runtime_error("File is too short!");
				}
					

				x = (i * texData.height + j) + (i * texData.height + j) + (i * texData.height + j);

				texData.data[x + 2] = fileArr[pos++];
				texData.data[x + 1] = fileArr[pos++];
				texData.data[x + 0] = fileArr[pos++];

			}

		return texData;
	}

	TextureDataStruct CreateTextureDataFromBmp32(const std::string& fullFileName, const std::string& ZIPFileName)
	{

		TextureDataStruct texData;
		std::vector<char> fileArr;
		
		if (!ZIPFileName.empty()) {
			fileArr = readFileFromZIP(fullFileName, ZIPFileName);
		}
		else {
			fileArr = readFile(fullFileName);
		}

		size_t fileSize = fileArr.size();

		if (fileSize < 22)
		{
			throw std::runtime_error("File is too short or not correct!");
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
					throw std::runtime_error("File is too short!");
				}


				x = (i * texData.height + j) + (i * texData.height + j) + (i * texData.height + j) + (i * texData.height + j);

				texData.data[x + 2] = fileArr[pos++];
				texData.data[x + 1] = fileArr[pos++];
				texData.data[x + 0] = fileArr[pos++];
				texData.data[x + 3] = fileArr[pos++];

			}

		return texData;
	}

#ifdef PNG_ENABLED

	TextureDataStruct CreateTextureDataFromPng(const std::string& fullFileName)
	{
		TextureDataStruct texData;

		FILE* file = fopen(fullFileName.c_str(), "rb");
		if (!file) {
			fclose(file);
			throw std::runtime_error("Could not open file " + fullFileName);
		}

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png) {
			fclose(file);
			throw std::runtime_error("Could not create PNG read structure");
		}

		png_infop info = png_create_info_struct(png);
		if (!info) {
			fclose(file);
			png_destroy_read_struct(&png, nullptr, nullptr);
			throw std::runtime_error("Could not create PNG info structure");
		}

		if (setjmp(png_jmpbuf(png))) {
			png_destroy_read_struct(&png, &info, nullptr);
			fclose(file);
			throw std::runtime_error("Error during PNG read");
		}

		png_init_io(png, file);
		png_read_info(png, info);

		texData.width = png_get_image_width(png, info);
		texData.height = png_get_image_height(png, info);
		png_byte color_type = png_get_color_type(png, info);
		png_byte bit_depth = png_get_bit_depth(png, info);

		if (bit_depth == 16)
			png_set_strip_16(png);

		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png);

		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(png);

		if (png_get_valid(png, info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		if (color_type == PNG_COLOR_TYPE_RGB ||
			color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

		if (color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		png_read_update_info(png, info);


		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * texData.height);
		for (int y = 0; y < texData.height; y++) {
			row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
		}

		png_read_image(png, row_pointers);

		fclose(file);

		bool has_alpha = (color_type & PNG_COLOR_MASK_ALPHA) || (png_get_valid(png, info, PNG_INFO_tRNS));

		size_t dataSize;

		if (has_alpha)
		{
			texData.bitSize = TextureDataStruct::BS_32BIT;
		}
		else
		{
			texData.bitSize = TextureDataStruct::BS_24BIT;
		}

		int channels = has_alpha ? 4 : 3;

		dataSize = texData.width * texData.height * channels;
		texData.data.resize(dataSize);

		
		for (int y = 0; y < texData.height; y++) {
			png_bytep row = row_pointers[y];
			for (int x = 0; x < texData.width; x++) {
				png_bytep px = &(row[x * 4]);
				texData.data[(y * texData.width + x) * channels + 0] = px[0]; // R
				texData.data[(y * texData.width + x) * channels + 1] = px[1]; // G
				texData.data[(y * texData.width + x) * channels + 2] = px[2]; // B
				if (has_alpha) {
					texData.data[(y * texData.width + x) * channels + 3] = px[3]; // A
				}
			}
			free(row_pointers[y]);
		}
		free(row_pointers);

		png_destroy_read_struct(&png, &info, nullptr);

		return texData;
	}
#endif
}