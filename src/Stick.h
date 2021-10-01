#pragma once

#include "GameObject.h"

class Stick : public GameObject {
public:
	Stick() : GameObject(), m_Pos(0) {}

	void SetPosition(float x, float y);
	void RotateStick(float x, float y);
	void HitBall();

private:
	glm::vec2 m_Pos;
};