#pragma once

#include "GameObject.h"
#include "glm/glm.hpp"

enum class BallColor {
	RED,
	BLUE,
	BLACK,
	WHITE
};

class Ball : public GameObject {
public:
	Ball() : GameObject(), m_Color(BallColor::RED) {}
	~Ball() = default;

	void CreateBall(Texture* tex, float radius, float xPos, float yPos, BallColor color);
	void ResetPosition();

public:
	float m_Radius = 0;
	BallColor m_Color;
	glm::vec2 m_StartPos = { 0, 0 };
	glm::vec2 m_Vel = { 0, 0 };
};