#pragma once

#include "GameObject.h"
#include <array>

class Table : public GameObject {
public:
	Table() : GameObject() {}
	~Table() = default;
	
private:
	bool m_Scored = false;

	std::array<glm::vec2, 6> m_Pockets;
};