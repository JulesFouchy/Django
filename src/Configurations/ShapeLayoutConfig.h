#pragma once

#include "OpenGL/ComputeShader.h"

class ShapeLayoutConfig {
public:
	ShapeLayoutConfig(const std::string& shapeFilepath, const std::string& layoutFilepath);
	~ShapeLayoutConfig() = default;

private:
	ComputeShader m_computeShader;
};