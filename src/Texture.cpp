#include "Texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

Texture::~Texture() 
{
	glDeleteTextures(1, &m_RenderID);
}

void Texture::CreateTexture(const char* path) 
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path, &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_RenderID);
	glBindTexture(GL_TEXTURE_2D, m_RenderID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Give OpenGL the data that was read
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

void Texture::Bind(unsigned int slot /* = 0 */) const 
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RenderID);
}

void Texture::Unbind() const 
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::DeleteTexture() const 
{
	glDeleteTextures(1, &m_RenderID);
}