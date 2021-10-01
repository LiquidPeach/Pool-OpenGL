#pragma once

#include "Ball.h"
#include "Stick.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include <array>

struct GLFWwindow;

class Pool {
public:
	Pool(GLFWwindow* window, float windowWidth, float windowHeight);
	~Pool() = default;

	void SetProjection();
	void InitGame();
	void WallCollision(Ball& ball);
	void BallCollision(Ball& ballA, Ball& ballB);
	void UpdatePosition(Ball& ball, float deltaTime);
	void SetMVP(GameObject& object);
	void DrawPool();

private:
	GLFWwindow* m_Window = 0;
	float m_WindowWidth = 0;
	float m_WindowHeight = 0;

	std::array<Ball, 16> m_Balls;
	Stick m_Stick;
	Shader m_Shader;
	glm::mat4 m_Model, m_View, m_Proj, m_MVP;
};