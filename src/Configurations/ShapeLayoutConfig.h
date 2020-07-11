#pragma once

#include "OpenGL/ComputeShader.h"

class ParticlesSystem;

class ShapeLayoutConfig {
public:
	ShapeLayoutConfig(const std::string& shapeFilepath, const std::string& layoutFilepath);
	~ShapeLayoutConfig() = default;

	void applyTo(ParticlesSystem& particlesSystem);

private:
	ComputeShader m_computeShader;
};