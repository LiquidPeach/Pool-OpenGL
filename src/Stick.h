#pragma once

#include "GameObject.h"

class Stick : public GameObject {
public:
	Stick() : GameObject() {}

	void SetStartPosition(float x, float y); // To be used everytime the cue ball is repositioned
	void RotateStick(float angle, glm::vec2 pivotPoint, glm::vec2 mouse);
	void PullStick(glm::vec2 mouse);

public:
	float m_Angle = 0;
	float m_Force = 0;
	float m_BallDist = 0;

	glm::vec2 m_Pos       = { 0, 0 };
	glm::vec2 m_StartPos  = { 0, 0 };
	glm::vec2 m_TempPos	  = { 0, 0 };

	glm::vec2 m_ClickPos  = { 0, 0 };
	glm::vec2 m_MouseDist = { 0, 0 };
	glm::vec2 m_Pivot	  = { 0, 0 };
};