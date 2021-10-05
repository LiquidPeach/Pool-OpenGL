#include "Pool.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <math.h>

#include <iostream>

static float s_DeltaTime = 0.0f;
static float s_LastFrame = 0.0f;

static int s_White = 0;
static int s_Black = 0;

Pool::Pool(GLFWwindow* window, float windowWidth, float windowHeight)
	: m_Model(0), m_View(0), m_Proj(0), m_MVP(0)
{
	m_Window = window;
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	m_Shader.CreateProgram("src/shaders/defaultVert.vert", "src/shaders/defaultFrag.frag");

	SetProjection();
	InitGame();
}

void Pool::SetProjection()
{
	m_Shader.Bind();

	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	m_Proj = glm::ortho(0.0f, m_WindowWidth, 0.0f, m_WindowHeight, -1.0f, 1.0f);

	m_MVP = m_Proj * m_View * m_Model;
	m_Shader.SetUniformMatrix4fv("u_MVP", 1, GL_FALSE, &m_MVP[0][0]);

	m_Shader.Unbind();
}

void Pool::InitGame() // Ball and stick setup
{
	float xPos = m_WindowWidth - 300;
	float yPos = m_WindowHeight / 2;
	float size = 30.0f;
	float radius = size / 2.0f;

	// Row 1
	m_Balls[0].CreateBall("res/redBall.png", radius, xPos, yPos, BallColor::RED);
	// Row 2
	m_Balls[1].CreateBall("res/blueBall.png", radius, xPos + size, yPos - radius, BallColor::BLUE);
	m_Balls[2].CreateBall("res/redBall.png", radius, xPos + size, yPos + radius, BallColor::RED);

	// Row 3 with Black Ball
	m_Balls[3].CreateBall("res/blueBall.png", radius, xPos + (size * 2), yPos - (radius * 2), BallColor::BLUE);
	m_Balls[4].CreateBall("res/blackBall.png", radius, xPos + (size * 2), yPos, BallColor::BLACK);
	s_Black = 4;
	m_Balls[5].CreateBall("res/redBall.png", radius, xPos + (size * 2), yPos + (radius * 2), BallColor::RED);

	// Row 4
	m_Balls[6].CreateBall("res/blueBall.png", radius, xPos + (size * 3), yPos - (radius * 3), BallColor::BLUE);
	m_Balls[7].CreateBall("res/redBall.png", radius, xPos + (size * 3), yPos - radius, BallColor::RED);
	m_Balls[8].CreateBall("res/blueBall.png", radius, xPos + (size * 3), yPos + radius, BallColor::BLUE);
	m_Balls[9].CreateBall("res/redBall.png", radius, xPos + (size * 3), yPos + (radius * 3), BallColor::RED);

	// Row 5
	m_Balls[10].CreateBall("res/redBall.png", radius, xPos + (size * 4), yPos - (radius * 4), BallColor::RED);
	m_Balls[11].CreateBall("res/blueBall.png", radius, xPos + (size * 4), yPos - (radius * 2), BallColor::BLUE);
	m_Balls[12].CreateBall("res/blueBall.png", radius, xPos + (size * 4), yPos, BallColor::BLUE);
	m_Balls[13].CreateBall("res/redBall.png", radius, xPos + (size * 4), yPos + (radius * 2), BallColor::RED);
	m_Balls[14].CreateBall("res/blueBall.png", radius, xPos + (size * 4), yPos + (radius * 4), BallColor::BLUE);

	// White ball
	m_Balls[15].CreateBall("res/whiteBall.png", radius, 180.0f, yPos, BallColor::WHITE);
	s_White = 15;

	// Stick
	m_Stick.CreateGameObject("res/stick.png", 650.0f, 25.0f);
}

void Pool::WallCollision(Ball& ball)
{
	if (ball.m_Pos.x >= (m_WindowWidth - ball.m_Radius) || ball.m_Pos.x <= ball.m_Radius)
		ball.m_Momen.x *= -1;
	if (ball.m_Pos.y >= (m_WindowHeight - ball.m_Radius) || ball.m_Pos.y <= ball.m_Radius)
		ball.m_Momen.y *= -1;
}

void Pool::BallCollision(Ball& ballA, Ball& ballB) // Subtract the distance between two balls with their summed radii
{
	glm::vec2 dist = ballB.m_Pos - ballA.m_Pos;
	float R = ballB.m_Radius + ballA.m_Radius;

	float magDist = static_cast<float>(sqrt( (dist.x * dist.x) + (dist.y * dist.y) ));

	glm::vec2 forceA(0);
	glm::vec2 forceB(0);

	if (magDist <= R) 
	{
		float k = 10.0f;
		forceA = k * (magDist - R) * glm::normalize(dist);
		forceB = -1.0f * forceA;
	}
	ballA.m_Momen += forceA;
	ballB.m_Momen += forceB;
}

void Pool::UpdateBallPosition(Ball& ball, float deltaTime)
{
	float magMomen = sqrt( (ball.m_Momen.x * ball.m_Momen.x) + (ball.m_Momen.y * ball.m_Momen.y) );
	if (magMomen < 2.6f)
		ball.m_Momen = { 0.0f, 0.0f };

	float x = ball.m_Pos.x + ball.m_Momen.x * deltaTime;
	float y = ball.m_Pos.y + ball.m_Momen.y * deltaTime;
	ball.m_Momen *= 0.9998f; // Friction

	ball.SetPosition(x, y);
}

void Pool::UpdateStickPosition()
{
	glm::vec2 mousePos = GetMousePosition();

	float xDiff = mousePos.x - m_Balls[s_White].m_Pos.x;
	float yDiff = mousePos.y - m_Balls[s_White].m_Pos.y;

	float x = m_Stick.m_Pos.x - m_Balls[s_White].m_Pos.x;
	float y = m_Stick.m_Pos.y - m_Balls[s_White].m_Pos.y;
	glm::vec2 h(x, y);

	m_Stick.RotateStick(xDiff, yDiff, m_Balls[s_White].m_Pos);
}

glm::vec2 Pool::GetMousePosition()
{
	double xPos, yPos;
	glfwGetCursorPos(m_Window, &xPos, &yPos); // returns coordinates relative to top left corner of window

	// since projection matrix is relative to bottom left corner of screen
	yPos = m_WindowHeight - yPos;

	glm::vec2 mousePos(0);
	mousePos.x = float(xPos);
	mousePos.y = float(yPos);

	return mousePos;
}

void Pool::SetMVP(GameObject& object)
{
	m_MVP = m_Proj * m_View * object.GetModelMatrix();
	m_Shader.SetUniformMatrix4fv("u_MVP", 1, GL_FALSE, &m_MVP[0][0]);
}

void Pool::DrawPool()
{
	m_Shader.Bind();

	float time = static_cast<float>(glfwGetTime());
	s_DeltaTime = time - s_LastFrame;
	s_LastFrame = time;

	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS) {
		m_Balls[s_White].m_Momen.x = 1000.0f;
		m_Balls[s_White].m_Momen.y = 100.0f;
	}
		
	for (int i = 0; i < m_Balls.size(); i++) 
	{
		for (int j = i + 1; j < m_Balls.size(); j++)
			BallCollision(m_Balls[i], m_Balls[j]);

		WallCollision(m_Balls[i]);
		UpdateBallPosition(m_Balls[i], s_DeltaTime);

		SetMVP(m_Balls[i]);
		m_Balls[i].Draw();
	}
	// TODO - check if all balls are stopped before drawing the stick
	if (m_Balls[s_White].m_Momen.x == 0 && m_Balls[s_White].m_Momen.y == 0) 
	{
		float x = m_Balls[s_White].m_Pos.x - (m_Stick.GetWidth() / 2) - m_Balls[s_White].m_Radius;
		m_Stick.SetStartPosition(x, m_Balls[s_White].m_Pos.y);

		UpdateStickPosition();
		SetMVP(m_Stick);
		m_Stick.Draw();
	}

	m_Shader.Unbind();
}