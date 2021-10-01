#pragma once

#include <iostream>
#include <unordered_map>

class Shader {
public:
	Shader() = default;
	~Shader();

	void CompileShader(const unsigned int shaderType, const char* source);
	void CreateProgram(const std::string& vertexShader, const std::string& fragmentShader);
	std::string ReadFile(const std::string& fileName);

	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4fv(const std::string& name, int count, unsigned int transpose, float* value);
	int GetUniformLocation(const std::string& name);

	void Bind() const;
	void Unbind() const;
	void DeleteShader() const;

private:
	unsigned int m_ProgramID = 0;
	std::unordered_map<std::string, int> m_UniformLocationCache;
};