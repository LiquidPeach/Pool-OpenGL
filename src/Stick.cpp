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
	m_Force = 0;

	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
}

void Stick::RotateStick(float angle, glm::vec2 pivotPoint, glm::vec2 mouse)
{
	m_Angle = angle;
	m_Pivot = pivotPoint;
	m_ClickPos = mouse;

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

	m_MouseToPos = mouse - m_Pos;
	m_TempPos = m_Pos;
}

void Stick::PullStick(glm::vec2 mouse)
{
	// Find point closest to the slope. y = mx + b 

	glm::vec2 mVec = m_ClickPos - m_Pivot;
	float m1 = mVec.y / mVec.x;
	float m2 = -mVec.x / mVec.y;

	float b1 = m_ClickPos.y - (m1 * m_ClickPos.x);
	float b2 = mouse.y - (m2 * mouse.x);

	float mouseX = (b2 - b1) / (m1 - m2);
	float mouseY = (m1 * mouseX) + b1;

	// Calculate new Cue stick position

	m_TempPos.x = mouseX - m_MouseToPos.x;
	m_TempPos.y = mouseY - m_MouseToPos.y;

	glm::vec2 distFromBall = m_TempPos - m_Pivot; // distance from the potential stick position to the cue ball
	float ballDist = sqrtf(powf(distFromBall.x, 2) + powf(distFromBall.y, 2));

	float diffBallDist = ballDist - m_BallDist;

	glm::vec2 mouseToClick = { mouseX - m_ClickPos.x, mouseY - m_ClickPos.y };
	float distFromClick = sqrtf(powf(mouseToClick.x, 2) + powf(mouseToClick.y, 2));


	if (diffBallDist >= 0 && distFromClick <= 100.0f)
	{
		m_Pos = m_TempPos;
		m_Force = diffBallDist * 15.0f;
	}
	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0));
	m_Model *= glm::rotate(glm::mat4(1.0f), m_Angle, glm::vec3(0.0f, 0.0f, 1.0f));
}