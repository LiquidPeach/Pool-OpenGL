#include "Pool.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <math.h>

Pool::Pool(GLFWwindow* window, unsigned int windowWidth, unsigned int windowHeight)
	: m_View(0), m_Proj(0)
{
	m_Window = window;
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	m_Shader.CreateProgram("src/shaders/defaultVert.vert", "src/shaders/defaultFrag.frag");

	m_RedTex.CreateTexture("res/redBall.png");
	m_BlueTex.CreateTexture("res/blueBall.png");
	m_BlackTex.CreateTexture("res/blackBall.png");
	m_WhiteTex.CreateTexture("res/whiteBall.png");
	m_CueTex.CreateTexture("res/stick.png");
	m_TableTex.CreateTexture("res/table.png");

	SetProjection();
	InitGame();
	SetTable();
}

void Pool::SetProjection()
{
	m_Shader.Bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	m_Proj = glm::ortho(0.0f, (float)m_WindowWidth, 0.0f, (float)m_WindowHeight, -1.0f, 1.0f);

	glm::mat4 MVP = m_Proj * m_View * model;
	m_Shader.SetUniformMatrix4fv("u_MVP", 1, GL_FALSE, &MVP[0][0]);

	m_Shader.Unbind();
}

void Pool::InitGame() // Ball and stick setup
{
	float xPos = m_WindowWidth - 300.0f;
	float yPos = m_WindowHeight / 2.0f;
	float size = 30.0f;
	float radius = size / 2.0f;

	// Row 1
	m_Balls[0].CreateBall(&m_RedTex, radius, xPos, yPos, BallColor::RED);
	// Row 2
	m_Balls[1].CreateBall(&m_BlueTex, radius, xPos + size, yPos - radius, BallColor::BLUE);
	m_Balls[2].CreateBall(&m_RedTex, radius, xPos + size, yPos + radius, BallColor::RED);
	// Row 3 with Black Ball
	m_Balls[3].CreateBall(&m_BlueTex, radius, xPos + (size * 2), yPos - (radius * 2), BallColor::BLUE);
	m_Balls[4].CreateBall(&m_BlackTex, radius, xPos + (size * 2), yPos, BallColor::BLACK);
	m_Black = 4;
	m_Balls[5].CreateBall(&m_RedTex, radius, xPos + (size * 2), yPos + (radius * 2), BallColor::RED);
	// Row 4
	m_Balls[6].CreateBall(&m_BlueTex, radius, xPos + (size * 3), yPos - (radius * 3), BallColor::BLUE);
	m_Balls[7].CreateBall(&m_RedTex, radius, xPos + (size * 3), yPos - radius, BallColor::RED);
	m_Balls[8].CreateBall(&m_BlueTex, radius, xPos + (size * 3), yPos + radius, BallColor::BLUE);
	m_Balls[9].CreateBall(&m_RedTex, radius, xPos + (size * 3), yPos + (radius * 3), BallColor::RED);
	// Row 5
	m_Balls[10].CreateBall(&m_RedTex, radius, xPos + (size * 4), yPos - (radius * 4), BallColor::RED);
	m_Balls[11].CreateBall(&m_BlueTex, radius, xPos + (size * 4), yPos - (radius * 2), BallColor::BLUE);
	m_Balls[12].CreateBall(&m_BlueTex, radius, xPos + (size * 4), yPos, BallColor::BLUE);
	m_Balls[13].CreateBall(&m_RedTex, radius, xPos + (size * 4), yPos + (radius * 2), BallColor::RED);
	m_Balls[14].CreateBall(&m_BlueTex, radius, xPos + (size * 4), yPos + (radius * 4), BallColor::BLUE);
	// Cue ball
	m_Balls[15].CreateBall(&m_WhiteTex, radius, 180.0f, yPos, BallColor::WHITE);
	m_Cue = 15;
	// Cue Stick
	m_Stick.CreateGameObject(&m_CueTex, 625.0f, 25.0f);
	// Pool Table
	m_Table.CreateGameObject(&m_TableTex, (float)m_WindowWidth, (float)m_WindowHeight);
	m_Table.SetPosition(m_WindowWidth / 2.0f, m_WindowHeight / 2.0f);
}

void Pool::SetTable()
{
	m_Table.m_PocketRadius = 17.0f;
	// Where the pockets are on the table. Starting on top left, going clockwise.
	m_Table.m_Pockets[0] = { 45.0f,  556.0f };
	m_Table.m_Pockets[1] = { 450.0f, 565.0f };
	m_Table.m_Pockets[2] = { 854.0f, 556.0f };
	m_Table.m_Pockets[3] = { 854.0f, 51.0f  };
	m_Table.m_Pockets[4] = { 450.0f, 41.0f  };
	m_Table.m_Pockets[5] = { 45.0f,  51.0f  };

	// Corners of the bounds of the table
	m_Table.m_Corners[0] = { 43.0f,  561.0f };
	m_Table.m_Corners[1] = { 857.0f, 561.0f };
	m_Table.m_Corners[2] = { 857.0f, 45.0f  };
	m_Table.m_Corners[3] = { 43.0f,  45.0f  };
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

void Pool::BumperCollision(Ball& ball)
{
	// Bounds of the table (cushions)
	float xMin = m_Table.m_Corners[(int)Corners::BOTTOMLEFT].x;
	float xMax = m_Table.m_Corners[(int)Corners::BOTTOMRIGHT].x;

	float yMin = m_Table.m_Corners[(int)Corners::BOTTOMLEFT].y;
	float yMax = m_Table.m_Corners[(int)Corners::TOPLEFT].y;

	// Check Top and Bottom
	if (ball.m_Pos.x - ball.m_Radius >= xMin && ball.m_Pos.x + ball.m_Radius <= xMax)
	{
		if(ball.m_Pos.y - ball.m_Radius <= yMin || ball.m_Pos.y + ball.m_Radius >= yMax)
			ball.m_Vel.y *= -1;
	}
	// Check Left and Right
	if (ball.m_Pos.y - ball.m_Radius >= yMin && ball.m_Pos.y + ball.m_Radius <= yMax)
	{
		if (ball.m_Pos.x - ball.m_Radius <= xMin || ball.m_Pos.x + ball.m_Radius >= xMax)
			ball.m_Vel.x *= -1;
	}
}

void Pool::BallCollision(Ball& ballA, Ball& ballB) // Subtract the distance between two balls with their summed radii
{
	glm::vec2 dist = ballB.m_Pos - ballA.m_Pos;
	float R = ballB.m_Radius + ballA.m_Radius;

	float magDist = sqrtf(powf(dist.x, 2) + powf(dist.y, 2));

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
	float magVel = sqrtf(powf(ball.m_Vel.x, 2) + powf(ball.m_Vel.y, 2));
	if (magVel <= 2.5f)
		ball.m_Vel = { 0, 0 };

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

void Pool::HitCueBall()
{
	float angle = m_Stick.m_Angle;

	float x = m_Stick.m_Force * cos(angle);
	float y = m_Stick.m_Force * sin(angle);

	m_Balls[m_Cue].m_Vel = { x, y };
}

bool Pool::GoalCheck(const Ball& ball)
{
	float R = ball.m_Radius + m_Table.m_PocketRadius;

	for (int i = 0; i < m_Table.m_Pockets.size(); i++)
	{
		glm::vec2 dist = ball.m_Pos - m_Table.m_Pockets[i];
		float magDist = sqrtf(powf(dist.x, 2) + powf(dist.y, 2));

		if (magDist <= R)
			return true;
	}
	return false;
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

	return atan2f(yDiff, xDiff);
}

void Pool::SetMVP(GameObject& object)
{
	glm::mat4 MVP = m_Proj * m_View * object.GetModelMatrix();
	m_Shader.SetUniformMatrix4fv("u_MVP", 1, GL_FALSE, &MVP[0][0]);
}

void Pool::DrawPool()
{
	m_Shader.Bind();

	SetMVP(m_Table);
	m_Table.Draw();

	float time = (float)glfwGetTime();
	m_DeltaTime = time - m_LastFrame;
	m_LastFrame = time;

	// Render Balls

	int ballCount = 0;
	for (int i = 0; i < m_Balls.size(); i++) 
	{
		for (int j = i + 1; j < m_Balls.size(); j++)
			BallCollision(m_Balls[i], m_Balls[j]);

		m_Goal = GoalCheck(m_Balls[i]);
		BumperCollision(m_Balls[i]);
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
		m_Moving = (m_Moving == false && ballCount == m_Balls.size()) ? false : true;

		SetMVP(m_Balls[i]);
		m_Balls[i].Draw();
	}

	// Render Cue Stick

	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS && m_Moving == false)
	{
		double xMouse, yMouse;
		glfwGetCursorPos(m_Window, &xMouse, &yMouse);
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