#include "Table.h"

#include "glm/gtc/matrix_transform.hpp"

Table::Table()
	: GameObject()
{
	for (int i = 0; i < 6; i++)
		m_Pockets[i] = { 0, 0 };

	for (int i = 0; i < 4; i++)
		m_Corners[i] = { 0, 0 };
}
