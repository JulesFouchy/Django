#pragma once

#include "RectSizePos.h"

class RectSizePosConstrainedRatio {
	friend class Viewports;
public:
	RectSizePosConstrainedRatio(int width = 0, int height = 0, int topLeftX = 0, int topLeftY = 0)
		: m_sizePos(width, height, topLeftX, topLeftY)
	{}

	inline bool isRatioConstrained() const { return m_bIsRatioConstrained; }

	glm::ivec2 topLeft() const;                                                            // y-axis points down
	inline glm::ivec2 botRight() const { return topLeft() + size(); }                      // y-axis points down
	inline glm::ivec2 botLeft() const  { return { topLeft().x, topLeft().y + size().y }; } // y-axis points down
	inline glm::ivec2 topRight() const { return { topLeft().x + size().x, topLeft().y }; } // y-axis points down

	glm::ivec2 size() const;
	inline int width() const  { return size().x; }
	inline int height() const { return size().y; }
	inline float aspectRatio() const { return isRatioConstrained() ? m_desiredRatio : m_sizePos.aspectRatio(); }

private:
	inline void constrainRatio(float desiredRatio) { m_desiredRatio = desiredRatio; m_bIsRatioConstrained = true; }
	inline void unconstrainRatio() { m_bIsRatioConstrained = false; }

	inline void setUnconstrainedTopLeft(int topLeftX, int topLeftY) { m_sizePos.setTopLeft(topLeftX, topLeftY); }
	inline void setUnconstrainedSize(int width, int height) { m_sizePos.setSize(width, height); }

	inline const glm::ivec2& unconstrainedSize() const { return m_sizePos.size(); }

private:
	RectSizePos m_sizePos;
	float m_desiredRatio = 1.0f;
	bool m_bIsRatioConstrained = false;
};