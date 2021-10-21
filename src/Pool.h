#pragma once

#include "Ball.h"
#include "Stick.h"
#include "Shader.h"
#include "glm/glm.hpp"

struct GLFWwindow;

class Pool {
public:
	Pool(GLFWwindow* window, float windowWidth, float windowHeight);
	~Pool() = default;

	void SetProjection();
	void InitGame();

	void WallCollision(Ball& ball);
	void BallCollision(Ball& ballA, Ball& ballB);
	void UpdateBallPosition(Ball& ball, float deltaTime);
	void UpdateStickPosition();
	float GetMouseAngle();

	void HitCueBall();
	void SetMVP(GameObject& object); // for switching back and forth between MVP's
	void DrawPool();

private:
	GLFWwindow* m_Window = 0;
	float m_WindowWidth  = 0;
	float m_WindowHeight = 0;

	// Index of the cue and black ball in the array of Balls
	int m_Cue = 0;
	int m_Black = 0;

	float m_DeltaTime = 0;
	float m_LastFrame = 0;
	bool  m_Moving = false;

	Shader m_Shader;
	Stick  m_Stick;
	std::array<Ball, 16> m_Balls;

	glm::vec2 m_MousePos = { 0, 0 };
	glm::mat4 m_View, m_Proj;
};