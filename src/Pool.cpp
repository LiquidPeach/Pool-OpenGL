#include "Pool.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <math.h>

#include <iostream>

static float s_DeltaTime = 0.0f;
static float s_LastFrame = 0.0f;

// Index of the cue and black ball in the array of Balls
static int s_Cue = 0;
static int s_Black = 0;

static bool s_Moving = false; // To check if every ball is moving

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
	s_Cue = 15;

	// Stick
	m_Stick.CreateGameObject("res/stick.png", 625.0f, 25.0f);
}

void Pool::WallCollision(Ball& ball)
{
	if (ball.m_Pos.x >= (m_WindowWidth - ball.m_Radius) || ball.m_Pos.x <= ball.m_Radius)
		ball.m_Vel.x *= -1;
	if (ball.m_Pos.y >= (m_WindowHeight - ball.m_Radius) || ball.m_Pos.y <= ball.m_Radius)
		ball.m_Vel.y *= -1;
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
	ballA.m_Vel += forceA;
	ballB.m_Vel += forceB;
}

void Pool::UpdateBallPosition(Ball& ball, float deltaTime)
{
	double magVel = sqrt( pow(ball.m_Vel.x, 2) + pow(ball.m_Vel.y, 2) );
	if (magVel <= 2.5)
		ball.m_Vel = { 0.0f, 0.0f };

	ball.m_Vel *= 0.9995f; // Friction
	ball.m_Momen = ball.m_Mass * ball.m_Vel;

	float x = ball.m_Pos.x + ball.m_Momen.x * deltaTime;
	float y = ball.m_Pos.y + ball.m_Momen.y * deltaTime;
	ball.SetPosition(x, y);
}

void Pool::UpdateStickPosition()
{
	float angle = GetMouseAngle();
	m_Stick.RotateStick(angle, m_Balls[s_Cue].m_Pos, m_MousePos);
}

float Pool::GetMouseAngle()
{
	double xMouse, yMouse;
	glfwGetCursorPos(m_Window, &xMouse, &yMouse); // Returns coordinates relative to top left corner of window...
	// ...so we must make it relative to the bottom left corner, since this is the origin according to the projection matrix
	yMouse = m_WindowHeight - yMouse;

	m_MousePos = { (float)xMouse, (float)yMouse };
	
	// Find the distance from the mouse to the cue ball
	float xDiff = (float)xMouse - m_Balls[s_Cue].m_Pos.x;
	float yDiff = (float)yMouse - m_Balls[s_Cue].m_Pos.y;

	return atan2f(yDiff, xDiff); // return angle in radians
}

void Pool::HitCueBall()
{
	float angle = m_Stick.m_Angle;

	float x = m_Stick.m_Force * cos(angle) * 100.0f;
	float y = m_Stick.m_Force * sin(angle) * 100.0f;

	m_Balls[s_Cue].m_Vel = { x, y };
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

	// Render Balls

	int ballCount = 0;
	for (int i = 0; i < m_Balls.size(); i++) 
	{
		for (int j = i + 1; j < m_Balls.size(); j++)
			BallCollision(m_Balls[i], m_Balls[j]);

		WallCollision(m_Balls[i]);
		UpdateBallPosition(m_Balls[i], s_DeltaTime);

		if (m_Balls[i].m_Vel.x == 0 && m_Balls[i].m_Vel.y == 0) 
		{
			s_Moving = false;
			ballCount++;
		} 
		else 
		{
			s_Moving = true;
			ballCount = 0;
		}
		if (s_Moving == false && ballCount == m_Balls.size())
			s_Moving = false;
		else
			s_Moving = true;

		SetMVP(m_Balls[i]);
		m_Balls[i].Draw();
	}

	// Render Cue Stick

	// TODO - drag stick with mouse instead of Down key
	//int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
	int downState = glfwGetKey(m_Window, GLFW_KEY_DOWN);
	if (downState == GLFW_PRESS && s_Moving == false)
	{
		m_Stick.PullStick();

		SetMVP(m_Stick);
		m_Stick.Draw();
	}
	else if (downState == GLFW_RELEASE && s_Moving == false)
	{
		HitCueBall();
		m_Stick.m_Force = 0.0f; // Reset force back to 0 after each hit

		// Set position of the cue stick and draw it
		float x = m_Balls[s_Cue].m_Pos.x - (m_Stick.GetWidth() / 2) - m_Balls[s_Cue].m_Radius;
		m_Stick.SetStartPosition(x, m_Balls[s_Cue].m_Pos.y);

		UpdateStickPosition();
		SetMVP(m_Stick);
		m_Stick.Draw();
	}

	m_Shader.Unbind();
}