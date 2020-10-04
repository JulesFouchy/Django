#include "RectSizePosConstrainedRatio.h"

glm::ivec2 RectSizePosConstrainedRatio::topLeft() const {
	if (isRatioConstrained()) {
		glm::ivec2 center = (m_sizePos.topLeft() + m_sizePos.botRight()) / 2;
		return center - size() / 2;
	}
	else {
		return m_sizePos.topLeft();
	}
}

glm::ivec2 RectSizePosConstrainedRatio::size() const {
	if (isRatioConstrained()) {
		if (m_desiredRatio > m_sizePos.aspectRatio())
			return { m_sizePos.width(), static_cast<int>(m_sizePos.width() / m_desiredRatio) };
		else
			return { static_cast<int>(m_sizePos.height() * m_desiredRatio), m_sizePos.height() };
	}
	else {
		return m_sizePos.size();
	}
}