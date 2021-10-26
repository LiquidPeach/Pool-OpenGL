#pragma once

#include "GameObject.h"
#include <array>

enum class Corners // Row indexes for bumper array
{
	TOPLEFT     = 0,
	TOPRIGHT    = 1,
	BOTTOMRIGHT = 2,
	BOTTOMLEFT  = 3
};

class Table : public GameObject {
public:
	Table();
	~Table() = default;
	
public:
	bool m_Scored = false;
	float m_PocketRadius = 0;
	std::array<glm::vec2, 6> m_Pockets;
	std::array<glm::vec2, 4> m_Corners;
};