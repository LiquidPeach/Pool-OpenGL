#pragma once

#include "Ball.h"
#include "Stick.h"
#include "Table.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include <array>

struct GLFWwindow;

class Pool {
public:
	Pool(GLFWwindow* window, unsigned int windowWidth, unsigned int windowHeight);
	~Pool() = default;

	void SetProjection();
	void InitGame();
	void SetTable();
	void ResetGame();

	void WallCollision(Ball& ball);
	void BallCollision(Ball& ballA, Ball& ballB);
	void UpdateBallPosition(Ball& ball, float deltaTime);
	void UpdateStickPosition();
	void HitCueBall();

	float GetMouseAngle();
	void SetMVP(GameObject& object);
	void DrawPool();

private:
	GLFWwindow*  m_Window = 0;
	unsigned int m_WindowWidth  = 0;
	unsigned int m_WindowHeight = 0;

	float m_DeltaTime = 0;
	float m_LastFrame = 0;
	bool  m_Moving = false;

	// Index of the cue and black ball in the array of Balls
	int m_Cue = 0;
	int m_Black = 0;

	glm::vec2 m_MousePos = { 0, 0 };
	glm::mat4 m_View, m_Proj;

	Texture m_BlueTex,
	        m_RedTex,
	        m_BlackTex,
	        m_WhiteTex,
	        m_CueTex,
	        m_TableTex;

	Shader  m_Shader;
	Stick   m_Stick;
	Table   m_Table;
	std::array<Ball, 16> m_Balls;
};