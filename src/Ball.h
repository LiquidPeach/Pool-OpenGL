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
	Ball() : GameObject(), m_Pos(0), m_Momen(0), m_Color(BallColor::RED) {}

	void CreateBall(const std::string& texSource, float radius, float xPos, float yPos, BallColor color);
	void SetPosition(float x, float y);
	void SetMomentum(float x, float y);

public:
	BallColor m_Color;
	glm::vec2 m_Pos;
	glm::vec2 m_Momen;
	float m_Radius = 0;
};