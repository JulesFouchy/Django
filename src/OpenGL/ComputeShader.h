#pragma once

#include "ShaderPipeline.h"

class ComputeShader {
public:
	ComputeShader(const std::string& filepath);
	~ComputeShader() = default;

	void compute(unsigned int nbComputations);
	inline ShaderPipeline& get() { return m_shader; }

private:
	ShaderPipeline m_shader;
};