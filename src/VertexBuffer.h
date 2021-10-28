#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
	VertexBuffer() = default;
	~VertexBuffer()
	{
		glDeleteBuffers(GL_ARRAY_BUFFER, &m_BufferID);
		m_BufferID = 0;
	}
	GLsizeiptr GetSize() const { return m_Size; }
	void CreateBuffer(float* vertices, GLsizeiptr size);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_BufferID = 0;
	GLsizeiptr m_Size = 0;
};