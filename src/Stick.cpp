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

void Stick::RotateStick(float angle, glm::vec2 pivotPoint, glm::vec2 mouse)
{
	m_Angle = angle;
	m_Mouse = mouse;
	m_Pivot = pivotPoint;

	float cosine = cos(m_Angle);
	float sine = sin(m_Angle);
	float cx = m_Pivot.x;
	float cy = m_Pivot.y;
	
	// rotate stick according to the desired distance between the ball and the stick
	m_Pos.x = ((m_StartPos.x - cx) * cosine - (m_StartPos.y - cy) * sine) + cx;
	m_Pos.y = ((m_StartPos.x - cx) * sine + (m_StartPos.y - cy) * cosine) + cy;

	// update the model matrix
	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos.x, m_Pos.y, 0));
	m_Model *= glm::rotate(glm::mat4(1.0f), m_Angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Stick::PullStick(glm::vec2 mouse)
{
	// OPTIMAIZATION - Find point closest to the slope
	// y = mx + b

	glm::vec2 mVec = m_Mouse - m_Pivot;
	float m1 = mVec.y / mVec.x;
	float m2 = -mVec.x / mVec.y;

	float b1 = m_Mouse.y - (m1 * m_Mouse.x);
	float b2 = mouse.y - (m2 * mouse.x);

	float newX = (b2 - b1) / (m1 - m2);
	float newY = (m1 * newX) + b1;

	// TODO - make cue stick move up/down along with mouse
	/*
	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0));
	m_Model *= glm::rotate(glm::mat4(1.0f), m_Angle, glm::vec3(0.0f, 0.0f, 1.0f));
	*/
}