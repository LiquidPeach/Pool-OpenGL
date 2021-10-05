#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include <string>
#include <array>

class GameObject {
public:
	GameObject() : m_Model(0) {};
	~GameObject() = default;

	void CreateGameObject(const std::string& texSource, float width, float height);

	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }
	void SetModelMatrix(glm::mat4 model);
	glm::mat4 GetModelMatrix() const { return m_Model; }
	void Draw() const;

protected:
	VertexArray  m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer  m_EBO;
	Texture      m_Texture;

	float m_Width = 0;
	float m_Height = 0;
	glm::mat4 m_Model;
};