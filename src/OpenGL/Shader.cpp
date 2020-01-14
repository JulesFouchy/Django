#include "Shader.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>

#include "Debugging/Log.h"

Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, bool compileShader)
	: m_shaderId(-1), m_vertexShaderFilepath(vertexShaderFilepath), m_fragmentShaderFilepath(fragmentShaderFilepath),
	  m_bCreatedSuccessfully(true)
{
	if (compileShader) {
		compile();
	}
}

Shader::~Shader() {
	glDeleteProgram(m_shaderId);
}

void Shader::bind() {
	glUseProgram(m_shaderId);
}

void Shader::compile() {
	spdlog::info("[Compiling Shader] " + m_vertexShaderFilepath + " & " + m_fragmentShaderFilepath);
	if (m_shaderId != -1)
		glDeleteProgram(m_shaderId);
	m_shaderId = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, parseFile(m_vertexShaderFilepath));
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, parseFile(m_fragmentShaderFilepath));

	glAttachShader(m_shaderId, vs);
	glAttachShader(m_shaderId, fs);
	glLinkProgram(m_shaderId);
	glValidateProgram(m_shaderId);

	glDeleteShader(vs);
	glDeleteShader(fs);
	Log::separationLine();
}

//Uniforms

int Shader::getUniformLocation(const std::string& uniformName) {
	if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[uniformName];
	}

	int location = glGetUniformLocation(m_shaderId, uniformName.c_str());
	//if (location == -1) {
	//	spdlog::warn("[Shader] uniform '{}' doesn't exist !", name);
	//}
	m_UniformLocationCache[uniformName] = location;
	return location;
}

void Shader::setUniform1i(const std::string& uniformName, int v) {
	glUniform1i(getUniformLocation(uniformName), v);
}
void Shader::setUniform1f(const std::string& uniformName, float v) {
	glUniform1f(getUniformLocation(uniformName), v);
}
void Shader::setUniform2f(const std::string& uniformName, const glm::vec2& v) {
	glUniform2f(getUniformLocation(uniformName), v.x, v.y);
}
void Shader::setUniform3f(const std::string& uniformName, const glm::vec3& v) {
	glUniform3f(getUniformLocation(uniformName), v.x, v.y, v.z);
}
void Shader::setUniform4f(const std::string& uniformName, const glm::vec4& v) {
	glUniform4f(getUniformLocation(uniformName), v.x, v.y, v.z, v.w);
}
void Shader::setUniformMat4f(const std::string& uniformName, const glm::mat4& mat) {
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
}

/* Utilities to open files and compile shaders */

std::string Shader::parseFile(const std::string& filepath) {
	std::ifstream stream(filepath);
	if (!stream.is_open()) {
		spdlog::warn("Failed to open file |{}|", filepath);
		m_bCreatedSuccessfully = false;
		return "";
	}

	std::string str = "";
	std::string tempLine = "";
	while (getline(stream, tempLine)) {
		str += tempLine + '\n';
	}
	stream.close();
	return str;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//Debug
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		m_bCreatedSuccessfully = false;
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		spdlog::warn("Failed to compile {} {}", (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") , "shader");
		spdlog::warn(message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}
