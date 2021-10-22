#include "Pool.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <math.h>

Pool::Pool(GLFWwindow* window, float windowWidth, float windowHeight)
	: m_View(0), m_Proj(0)
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

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	m_Proj = glm::ortho(0.0f, m_WindowWidth, 0.0f, m_WindowHeight, -1.0f, 1.0f);

	glm::mat4 MVP = m_Proj * m_View * model;
	m_Shader.SetUniformMatrix4fv("u_MVP", 1, GL_FALSE, &MVP[0][0]);

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
	m_Black = 4;
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

	// Cue ball
	m_Balls[15].CreateBall("res/whiteBall.png", radius, 180.0f, yPos, BallColor::WHITE);
	m_Cue = 15;

	// Cue Stick
	m_Stick.CreateGameObject("res/stick.png", 625.0f, 25.0f);
}

void Pool::ResetGame()
{
	for (int i = 0; i < m_Balls.size(); i++)
	{
		m_Balls[i].ResetPosition();
	}

	float x = m_Balls[m_Cue].m_Pos.x - (m_Stick.GetWidth() / 2) - m_Balls[m_Cue].m_Radius;
	m_Stick.SetStartPosition(x, m_Balls[m_Cue].m_Pos.y);
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

	float magDist = sqrtf( powf(dist.x, 2) + powf(dist.y, 2) );

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
	float magVel = sqrtf( powf(ball.m_Vel.x, 2) + powf(ball.m_Vel.y, 2) );
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
	m_Stick.RotateStick(angle, m_Balls[m_Cue].m_Pos, m_MousePos);
}

float Pool::GetMouseAngle()
{
	double xMouse, yMouse;
	glfwGetCursorPos(m_Window, &xMouse, &yMouse); // Returns coordinates relative to top left corner of window...
	// ...so we must make it relative to the bottom left corner, since this is the origin according to the projection matrix
	yMouse = m_WindowHeight - yMouse;

	m_MousePos = { (float)xMouse, (float)yMouse };
	
	// Find the distance from the mouse to the cue ball
	float xDiff = (float)xMouse - m_Balls[m_Cue].m_Pos.x;
	float yDiff = (float)yMouse - m_Balls[m_Cue].m_Pos.y;

	return atan2f(yDiff, xDiff); // return angle in radians
}

void Pool::HitCueBall()
{
	float angle = m_Stick.m_Angle;

	float x = m_Stick.m_Force * cos(angle);
	float y = m_Stick.m_Force * sin(angle);

	m_Balls[m_Cue].m_Vel = { x, y };
}

void Pool::SetMVP(GameObject& object)
{
	glm::mat4 MVP = m_Proj * m_View * object.GetModelMatrix();
	m_Shader.SetUniformMatrix4fv("u_MVP", 1, GL_FALSE, &MVP[0][0]);
}

void Pool::DrawPool()
{
	m_Shader.Bind();

	float time = static_cast<float>(glfwGetTime());
	m_DeltaTime = time - m_LastFrame;
	m_LastFrame = time;

	// Render Balls

	int ballCount = 0;
	for (int i = 0; i < m_Balls.size(); i++) 
	{
		for (int j = i + 1; j < m_Balls.size(); j++)
			BallCollision(m_Balls[i], m_Balls[j]);

		WallCollision(m_Balls[i]);
		UpdateBallPosition(m_Balls[i], m_DeltaTime);

		if (m_Balls[i].m_Vel.x == 0 && m_Balls[i].m_Vel.y == 0) 
		{
			m_Moving = false;
			ballCount++;
		} 
		else 
		{
			m_Moving = true;
			ballCount = 0;
		}
		if (m_Moving == false && ballCount == m_Balls.size())
			m_Moving = false;
		else
			m_Moving = true;

		SetMVP(m_Balls[i]);
		m_Balls[i].Draw();
	}

	// Render Cue Stick

	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_PRESS && m_Moving == false)
	{
		double xMouse, yMouse;
		glfwGetCursorPos(m_Window, &xMouse, &yMouse); // Returns coordinates relative to top left corner of window...
		// ...so we must make it relative to the bottom left corner, since this is the origin according to the projection matrix
		yMouse = m_WindowHeight - yMouse;

		m_MousePos = { (float)xMouse, (float)yMouse };

		m_Stick.PullStick(m_MousePos);

		SetMVP(m_Stick);
		m_Stick.Draw();
	}
	else if (state == GLFW_RELEASE && m_Moving == false)
	{
		HitCueBall();
		m_Stick.m_Force = 0.0f; // Reset force back to 0 after each hit

		// Set position of the cue stick and draw it
		float x = m_Balls[m_Cue].m_Pos.x - (m_Stick.GetWidth() / 2) - m_Balls[m_Cue].m_Radius;
		m_Stick.SetStartPosition(x, m_Balls[m_Cue].m_Pos.y);

		glm::vec2 ballDist = m_Stick.m_Pos - m_Balls[m_Cue].m_Pos;
		m_Stick.m_BallDist = sqrtf(powf(ballDist.x, 2) + powf(ballDist.y, 2));

		UpdateStickPosition();
		SetMVP(m_Stick);
		m_Stick.Draw();
	}

	m_Shader.Unbind();
}