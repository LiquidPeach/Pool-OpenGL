#pragma once

#include "GameObject.h"
#include "Ball.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <array>

class Pool {
public:
	Pool(GLFWwindow* window, float windowWidth, float windowHeight);
	~Pool() = default;

	void SetProjection();
	void InitGame();
	void WallCollision(Ball& ball);
	void BallCollision(Ball& ballA, Ball& ballB);
	void UpdatePosition(Ball& ball, float deltaTime);
	void DrawPool();

private:
	float m_WindowWidth = 0;
	float m_WindowHeight = 0;
	GLFWwindow* m_Window = 0;

	std::array<Ball, 16> balls;
	Shader m_Shader;
	glm::mat4 m_Model, m_View, m_Proj, m_MVP;
};