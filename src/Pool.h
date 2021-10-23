#pragma once

#include "Ball.h"
#include "Stick.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"

#include <array>

struct GLFWwindow;

class Pool {
public:
	Pool(GLFWwindow* window, float windowWidth, float windowHeight);
	~Pool() = default;

	void SetProjection();
	void InitGame();
	void ResetGame();

	void WallCollision(Ball& ball);
	void BallCollision(Ball& ballA, Ball& ballB);
	void UpdateBallPosition(Ball& ball, float deltaTime);
	void UpdateStickPosition();
	float GetMouseAngle();

	void HitCueBall();
	void SetMVP(GameObject& object);
	void DrawPool();

private:
	GLFWwindow* m_Window = 0;
	float m_WindowWidth  = 0;
	float m_WindowHeight = 0;

	float m_DeltaTime = 0;
	float m_LastFrame = 0;
	bool  m_Moving = false;

	// Index of the cue and black ball in the array of Balls
	int m_Cue = 0;
	int m_Black = 0;

	Texture m_BlueTex;
	Texture m_RedTex;
	Texture m_BlackTex;
	Texture m_WhiteTex;
	Texture m_CueTex;

	Shader m_Shader;
	Stick m_Stick;
	std::array<Ball, 16> m_Balls;

	glm::vec2 m_MousePos = { 0, 0 };
	glm::mat4 m_View, m_Proj;
};