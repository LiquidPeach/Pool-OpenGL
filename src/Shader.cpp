#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>

Shader::~Shader()
{
	glDeleteProgram(m_ProgramID);
	m_ProgramID = 0;
}

std::string Shader::ReadFile(const std::string& fileName)
{
	std::ifstream file(fileName);
	std::string line;
	std::stringstream ss;

	if (!file.is_open()) {
		std::cout << "ERROR! Could not open file: " << fileName << '\n';
		return "";
	}
	while (getline(file, line))
		ss << line << '\n';

	file.close();

	return ss.str();
}

void Shader::CompileShader(unsigned int shaderType, const char* source)
{
	glShaderSource(shaderType, 1, &source, nullptr);
	glCompileShader(shaderType);

	int success;
	glGetShaderiv(shaderType, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderType, 512, nullptr, infoLog);
		std::cout << "ERROR! Could not compile shader! " << infoLog << '\n';
	}
}

void Shader::CreateProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	m_ProgramID = glCreateProgram();
	
	// Create Vertex Shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexSource = ReadFile(vertexShader);
	CompileShader(vertex, vertexSource.c_str());

	// Create Fragment Shader
	unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragSource = ReadFile(fragmentShader);
	CompileShader(frag, fragSource.c_str());

	glAttachShader(m_ProgramID, vertex);
	glAttachShader(m_ProgramID, frag);
	glLinkProgram(m_ProgramID);
	
	// Check if link was successfull
	int success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (!success) 
	{
		char infoLog[512];
		glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
		std::cout << "ERROR! Could not link shaders! " << infoLog << '\n';
	}

	glDeleteShader(vertex);
	glDeleteShader(frag);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMatrix4fv(const std::string& name, int count, unsigned int transpose, float* value)
{
	glUniformMatrix4fv(GetUniformLocation(name), count, transpose, value);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_ProgramID, name.c_str());

	if (location == -1)
		std::cout << "Error! Uniform " << name << " does not exit in shader!\n";

	m_UniformLocationCache[name] = location;

	return location;
}

void Shader::Bind() const
{
	glUseProgram(m_ProgramID);
}
void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::DeleteShader() const
{
	glDeleteProgram(m_ProgramID);
}