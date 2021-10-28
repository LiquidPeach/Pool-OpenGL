#pragma once

#include "VertexBuffer.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();
	void LinkAttributes(const VertexBuffer& VBO, unsigned int layout, int size, GLenum type, GLsizei stride, const void* offset);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_ArrayID = 0;
};