#pragma once

#include <string>

enum class ShaderType {
	Vertex,
	Fragment,
	Compute
};

class Shader {
public:
	Shader(ShaderType type, const std::string& shaderFilepath);
	Shader(Shader&& other) noexcept;
	~Shader();

	inline unsigned int getID() const { return m_shaderID; }
	inline const std::string& getFilepath() const { return m_filepath; }

private:
friend class ShaderPipeline;
	void compile();
	std::string parseFile(const std::string& filepath);

private:
	unsigned int m_type;
	std::string m_filepath;
	unsigned int m_shaderID;
};