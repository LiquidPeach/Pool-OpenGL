#include "Table.h"

#include "glm/gtc/matrix_transform.hpp"

Table::Table()
	: GameObject()
{
	for (int i = 0; i < 6; i++)
		m_Pockets[i] = { 0, 0 };
}
