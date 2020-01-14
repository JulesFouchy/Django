#pragma once

#include <string>
#include <unordered_map>

#include <glm.hpp>

class Shader {
public:
	Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, bool compileShader = true);
	~Shader();

	void bind();
	void compile();

	inline bool createdSuccessfully() { return m_bCreatedSuccessfully; }

	inline unsigned int getID() const { return m_shaderId; }
	inline const std::string& getFragmentFilepath() const { return m_fragmentShaderFilepath; }

	//Uniforms
private:
	std::unordered_map<std::string, int> m_UniformLocationCache;
	int getUniformLocation(const std::string& uniformName);
public:
	void setUniform1i(const std::string& uniformName, int v);
	void setUniform1f(const std::string& uniformName, float v);
	void setUniform2f(const std::string& uniformName, const glm::vec2& v);
	void setUniform3f(const std::string& uniformName, const glm::vec3& v);
	void setUniform4f(const std::string& uniformName, const glm::vec4& v);
	void setUniformMat4f(const std::string& uniformName, const glm::mat4& mat);

private:
	unsigned int m_shaderId;
	std::string m_vertexShaderFilepath;
	std::string m_fragmentShaderFilepath;
	/* Utilities to open files and compile shaders */
	std::string parseFile(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);

	bool m_bCreatedSuccessfully;
};