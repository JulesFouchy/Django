#pragma once

#include "OpenGL/SSBO.h"

class SVGssbo {
public:
	SVGssbo();
	~SVGssbo() = default;

	std::string init(const std::string& svgFilepath); // returns the shader code that binds the buffer and sets the nbCurves variable

private:
	int m_id;
	SSBO m_ssbo;
	static int s_id;
};