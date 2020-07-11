#pragma once

#include "ShaderPipeline.h"

class ComputeShader {
public:
	ComputeShader() = default;
	ComputeShader(const std::string & filepath);
	~ComputeShader() = default;
	void initWithCode(const std::string& sourceCode);

	void compute(unsigned int nbComputations);
	inline ShaderPipeline& get() { return m_shader; }

private:
	ShaderPipeline m_shader;
};