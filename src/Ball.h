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

	void CreateBall(const std::string& texSource, float radius, float xPos, float yPos, BallColor color);
	void SetPosition(float x, float y);
	glm::vec2 GetPosition() const { return m_Pos; }
	void SetMomentum(float x, float y);

public:
	float m_Mass = 1.0f; // kgs
	float m_Radius = 0;
	BallColor m_Color;
	glm::vec2 m_Pos   = { 0.0f, 0.0f };
	glm::vec2 m_Vel   = { 0.0f, 0.0f };
	glm::vec2 m_Momen = { 0.0f, 0.0f };
};