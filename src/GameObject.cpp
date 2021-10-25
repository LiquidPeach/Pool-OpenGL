#include "GameObject.h"

#include "glm/gtc/matrix_transform.hpp"
#include <array>

GameObject::GameObject()
{
	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}

void GameObject::CreateGameObject(Texture* tex, float width, float height)
{
	m_Width = width;
	m_Height = height;
	m_Texture = tex;

	float vertices[] = {
		// Positions               // Texture
		-m_Width/2, -m_Height/2,   0.0f, 0.0f,
		 m_Width/2, -m_Height/2,   1.0f, 0.0f,
		 m_Width/2,  m_Height/2,   1.0f, 1.0f,
		-m_Width/2,  m_Height/2,   0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_VAO.Bind();
	m_VBO.CreateBuffer(vertices, 16 * sizeof(float));
	m_EBO.CreateBuffer(indices, 6 * sizeof(float));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_VAO.LinkAttributes(m_VBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (const void*)0);
	m_VAO.LinkAttributes(m_VBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

	m_VAO.Unbind();
}

void GameObject::SetPosition(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
}

void GameObject::SetModelMatrix(const glm::mat4& model)
{
	m_Model = model;
}

void GameObject::Draw() const
{
	m_VAO.Bind();
	m_Texture->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	m_Texture->Unbind();
	m_VAO.Unbind();
}