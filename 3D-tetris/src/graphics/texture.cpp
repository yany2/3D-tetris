#include <glad/gl.h>
#include "stb_image.h"

module graphics;

graphics::Texture::Texture(const char* textureLocation)
{
	glGenTextures(1, &texture);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(textureLocation, &width, &height, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

graphics::Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void graphics::Texture::use(Shader* shader) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setUniform("tex", 0);
}