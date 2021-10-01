#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void LinkAttributes(const VertexBuffer& VBO, unsigned int layout, int size, GLenum type, GLsizei stride, const void* offset);
	void Bind() const;
	void Unbind() const;
	void DeleteArray() const;

private:
	unsigned int m_ArrayID;
};