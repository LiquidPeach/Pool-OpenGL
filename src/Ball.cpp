#include "Ball.h"
#include "glm/gtc/matrix_transform.hpp"

void Ball::CreateBall(const std::string& texSource, float radius, float xPos, float yPos, BallColor color)
{
	m_Color = color;
	m_Radius = radius;
	m_Width = radius * 2;
	m_Height = radius * 2;

	SetPosition(xPos, yPos);
	m_StartPos = m_Pos;

	float vertices[] = {
		// Positions				   // Texture
		-m_Width / 2, -m_Height / 2,   0.0f, 0.0f,
		 m_Width / 2, -m_Height / 2,   1.0f, 0.0f,
		 m_Width / 2,  m_Height / 2,   1.0f, 1.0f,
		-m_Width / 2,  m_Height / 2,   0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_VAO.Bind();
	m_VBO.CreateBuffer(vertices, sizeof(vertices));
	m_EBO.CreateBuffer(indices, sizeof(indices));

	m_Texture.CreateTexture(texSource);
	m_Texture.Bind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_VAO.LinkAttributes(m_VBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (const void*)0);
	m_VAO.LinkAttributes(m_VBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

	m_Texture.Unbind();
	m_VAO.Unbind();
}

void Ball::SetPosition(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
}

void Ball::ResetPosition()
{
	m_Pos = m_StartPos;
	m_Vel = m_Momen = { 0, 0 };
}

void Ball::SetMomentum(float x, float y)
{
	m_Momen.x = x;
	m_Momen.y = y;
}