#include "Ball.h"
#include "glm/gtc/matrix_transform.hpp"

void Ball::CreateBall(Texture* tex, float radius, float xPos, float yPos, BallColor color)
{
	m_Texture = tex;
	m_Radius  = radius;
	m_Width   = radius * 2;
	m_Height  = radius * 2;
	m_Color   = color;

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
	m_VBO.CreateBuffer(vertices, 16 * sizeof(float));
	m_EBO.CreateBuffer(indices, 6 * sizeof(float));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_VAO.LinkAttributes(m_VBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (const void*)0);
	m_VAO.LinkAttributes(m_VBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

	m_VAO.Unbind();
}

void Ball::ResetPosition()
{
	m_Pos = m_StartPos;
	m_Vel = { 0, 0 };
}