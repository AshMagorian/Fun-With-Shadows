#include "Texture.h"

#include "stb_image.h"
#include "Exception.h"

#include <iostream>

Texture::Texture(int width, int height)
{
	size.x = width;
	size.y = height;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}
/**
*Loads the texture from teh path specified. If the file is not found then an exception is thrown and an error message
*displays in the log. A blank texture is then stored as the texture instead
*/
Texture::Texture(std::string path)
{
	int w = 0;
	int h = 0;
	int channels = 0;
	unsigned char *data = stbi_load(path.c_str(), &w, &h, &channels, 4);
	try {
		if (!data)
		{
			throw Exception("Texture file not found, " + path + " cannot be loaded ");
		}
	}
	catch (Exception& e)
	{
		std::cout << "myEngine Exception: " << e.what() << std::endl;
	}

	size.x = w;
	size.y = h;

	// Create and bind a texture.
	//GLuint textureId = 0;
	glGenTextures(1, &id);
	if (!id)
	{
		throw std::exception();
	}
	glBindTexture(GL_TEXTURE_2D, id);

	// Set the texture wrapping and filter options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload the image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);
	// Free the loaded data because we now have a copy on the GPU
	free(data);
	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);
}