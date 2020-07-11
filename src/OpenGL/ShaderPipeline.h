#pragma once

#include "Shader.h"

class ShaderPipeline {
public:
	ShaderPipeline();
	~ShaderPipeline();

	void addShader(ShaderType type, const std::string& shaderFilepath);
	void addShaderBySrcCode(ShaderType type, const std::string& shaderSrc);
	void createProgram();

	void bind();
	void unbind();

	void setUniform1i(const std::string& uniformName, int v);
	void setUniform1f(const std::string& uniformName, float v);
	void setUniform2f(const std::string& uniformName, const glm::vec2& v);
	void setUniform2f(const std::string& uniformName, const float* v);
	void setUniform3f(const std::string& uniformName, const glm::vec3& v);
	void setUniform3f(const std::string& uniformName, float v1, float v2, float v3);
	void setUniform4f(const std::string& uniformName, const glm::vec4& v);
	void setUniformMat4f(const std::string& uniformName, const glm::mat4& mat);
private:
	int getUniformLocation(const std::string& uniformName);

private:
	std::unordered_map<std::string, int> m_UniformLocationCache;
	std::vector<Shader> m_shaders;
	unsigned int m_programID;
};