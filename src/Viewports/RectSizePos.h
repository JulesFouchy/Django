#pragma once

#include "RectSize.h"

class RectSizePos : public RectSize {
public:
	RectSizePos(int width = 0, int height = 0, int topLeftX = 0, int topLeftY = 0)
		: RectSize(width, height), m_topLeft(topLeftX, topLeftY)
	{}

	inline glm::ivec2 topLeft() const  { return m_topLeft; }
	inline glm::ivec2 botRight() const { return m_topLeft + size(); }

	inline void setPos(int topLeftX, int topLeftY) { m_topLeft.x = topLeftX; m_topLeft.y = topLeftY; }

private:
	glm::ivec2 m_topLeft;
};