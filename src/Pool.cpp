#include "Pool.h"
#include "glm/gtc/matrix_transform.hpp"
#include <math.h>

#include <iostream>

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

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

void Pool::InitGame() // Ball setup
{
	float xPos = m_WindowWidth - 300;
	float yPos = m_WindowHeight / 2;
	float size = 30.0f;
	float radius = size / 2.0f;
	
	// Row 1
	balls[0].CreateBall("src/images/redBall.png", radius, xPos, yPos, BallColor::RED);
	// Row 2
	balls[1].CreateBall("src/images/blueBall.png", radius, xPos + size, yPos - radius, BallColor::BLUE);
	balls[2].CreateBall("src/images/redBall.png",  radius, xPos + size, yPos + radius, BallColor::RED);
	// Row 3 with Black Ball
	balls[3].CreateBall("src/images/blueBall.png", radius, xPos + (size * 2), yPos - (radius * 2), BallColor::BLUE);
	balls[4].CreateBall("src/images/blackBall.png",radius, xPos + (size * 2), yPos, BallColor::BLACK);
	balls[5].CreateBall("src/images/redBall.png",  radius, xPos + (size * 2), yPos + (radius * 2), BallColor::RED);
	// Row 4
	balls[6].CreateBall("src/images/blueBall.png", radius, xPos + (size * 3), yPos - (radius * 3), BallColor::BLUE);
	balls[7].CreateBall("src/images/redBall.png",  radius, xPos + (size * 3), yPos - radius, BallColor::RED);
	balls[8].CreateBall("src/images/blueBall.png", radius, xPos + (size * 3), yPos + radius, BallColor::BLUE);
	balls[9].CreateBall("src/images/redBall.png",  radius, xPos + (size * 3), yPos + (radius * 3), BallColor::RED);
	// Row 5
	balls[10].CreateBall("src/images/redBall.png", radius, xPos + (size * 4), yPos - (radius * 4), BallColor::RED);
	balls[11].CreateBall("src/images/blueBall.png",radius, xPos + (size * 4), yPos - (radius * 2), BallColor::BLUE);
	balls[12].CreateBall("src/images/blueBall.png",radius, xPos + (size * 4), yPos, BallColor::BLUE);
	balls[13].CreateBall("src/images/redBall.png", radius, xPos + (size * 4), yPos + (radius * 2), BallColor::RED);
	balls[14].CreateBall("src/images/blueBall.png",radius, xPos + (size * 4), yPos + (radius * 4), BallColor::BLUE);
	// White ball
	balls[15].CreateBall("src/images/whiteBall.png", radius, 100.0f, yPos, BallColor::WHITE);
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
		float k = 1.0f;
		forceA = k * (magDist - R) * glm::normalize(dist);
		forceB = -1.0f * forceA;
	}
	ballA.m_Momen += forceA;
	ballB.m_Momen += forceB;
}

void Pool::UpdatePosition(Ball& ball, float deltaTime)
{
	float x = ball.m_Pos.x + ball.m_Momen.x * deltaTime;
	float y = ball.m_Pos.y + ball.m_Momen.y * deltaTime;
	ball.SetPosition(x, y);

	m_MVP = m_Proj * m_View * ball.GetModelMatrix();
	m_Shader.SetUniformMatrix4fv("u_MVP", 1, GL_FALSE, &m_MVP[0][0]);
}

void Pool::DrawPool()
{
	m_Shader.Bind();

	float time = static_cast<float>(glfwGetTime());
	deltaTime = time - lastFrame;
	lastFrame = time;

	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
		balls[15].m_Momen.x = 1000.0f;
		
	for (int i = 0; i < balls.size(); i++) 
	{
		for (int j = i + 1; j < balls.size(); j++)
			BallCollision(balls[i], balls[j]);

		WallCollision(balls[i]);
		UpdatePosition(balls[i], deltaTime);
		balls[i].Draw();
	}
	m_Shader.Unbind();
}