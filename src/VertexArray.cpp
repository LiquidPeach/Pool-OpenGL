#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ArrayID);
}
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ArrayID);
	m_ArrayID = 0;
}

void VertexArray::LinkAttributes(const VertexBuffer& VBO, unsigned int layout, int size, GLenum type, GLsizei stride, const void* offset)
{
	VBO.Bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
	VBO.Unbind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ArrayID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}