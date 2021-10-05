#pragma once

#include "GameObject.h"

class Stick : public GameObject {
public:
	Stick() : GameObject() {}

	void SetStartPosition(float x, float y); // to be used everytime the cue ball is repositioned
	void RotateStick(float x, float y, glm::vec2 pivotPoint);
	void HitBall(); // TODO - implement this method

public:
	glm::vec2 m_Pos      = { 0.0f, 0.0f };
	glm::vec2 m_StartPos = { 0.0f, 0.0f };
};