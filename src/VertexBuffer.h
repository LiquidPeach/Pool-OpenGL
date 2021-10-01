#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
	VertexBuffer() = default;
	~VertexBuffer();

	GLsizeiptr GetSize() const { return m_Size; }

	void CreateBuffer(float* vertices, GLsizeiptr size);
	void Bind() const;
	void Unbind() const;
	void DeleteBuffer() const;

private:
	unsigned int m_BufferID = 0;
	GLsizeiptr m_Size = 0;
};