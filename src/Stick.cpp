#include "Stick.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <math.h>

#include <iostream>

void Stick::SetStartPosition(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_StartPos = m_Pos;

	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
}

void Stick::RotateStick(float angle, glm::vec2 pivotPoint)
{
	float cosine = cos(angle);
	float sine = sin(angle);
	float cx = pivotPoint.x;
	float cy = pivotPoint.y;
	
	// rotate stick according to the desired distance between the ball and the stick
	m_Pos.x = ((m_StartPos.x - cx) * cosine - (m_StartPos.y - cy) * sine) + cx;
	m_Pos.y = ((m_StartPos.x - cx) * sine + (m_StartPos.y - cy) * cosine) + cy;

	// update the model matrix
	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos.x, m_Pos.y, 0));
	m_Model *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Stick::PullStick(float angle)
{
	// Increase or Decrease magnitude
}