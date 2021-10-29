#pragma once

#include "GameObject.h"
#include <array>

class Table : public GameObject {
public:
	Table();
	~Table() = default;
	
public:
	float m_xMin = 0, 
	      m_xMax = 0,
	      m_yMin = 0,
	      m_yMax = 0;
	float m_PocketRadius = 0;
	std::array<glm::vec2, 6> m_Pockets;
};