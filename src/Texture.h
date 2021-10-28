#pragma once

#include <string>

class Texture {
public:
	Texture() = default;
	~Texture();
	void CreateTexture(const char* path);
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	void DeleteTexture() const;

private:
	unsigned int m_RenderID = 0;
	int m_Width = 0;
	int m_Height = 0;
	int m_BPP = 0; // Bits Per Pixel
	unsigned char* m_LocalBuffer = 0;
};