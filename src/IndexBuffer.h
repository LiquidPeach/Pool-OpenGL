#pragma once

#include <glad/glad.h>

class IndexBuffer {
public:
	IndexBuffer() = default;
	~IndexBuffer()
	{
		glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_BufferID);
		m_BufferID = 0;
	}
	GLsizeiptr GetSize() const { return m_Size; }
	void CreateBuffer(unsigned int* indices, GLsizeiptr size);
	void Bind() const;
	void Unbind() const;
	void DeleteBuffer() const;

private:
	unsigned int m_BufferID = 0;
	GLsizeiptr m_Size = 0;
};