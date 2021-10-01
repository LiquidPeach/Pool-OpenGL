#include "Stick.h"

#include "glm/gtc/matrix_transform.hpp"

void Stick::SetPosition(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0)));
}