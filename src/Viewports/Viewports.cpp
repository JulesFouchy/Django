#include "Viewports.h"

RectSizePos Viewports::m_Window;
RectSizePos Viewports::m_AvailableAppView;
RectSize Viewports::m_Export;
RectSize Viewports::m_OutputWindow;

bool Viewports::m_bIsExporting = false;
bool Viewports::m_bIsOutputWindowOpen = false;

bool Viewports::m_bConstrainAppViewRatio = false;
float Viewports::m_appViewConstrainedRatio = 16.0f / 9.0f;

std::function<void()> Viewports::OnRenderSizeChanged = []() {};

RectSize Viewports::RenderSize() {
	if (m_bIsExporting)
		return m_Export;
	else if (m_bIsOutputWindowOpen)
		return m_OutputWindow;
	else
		return AppView();
}

void Viewports::setIsExporting(bool bIsExporting) {
	m_bIsExporting = bIsExporting;
	OnRenderSizeChanged();
}

void Viewports::setIsOutputWindowOpen(bool bIsOpen) {
	m_bIsOutputWindowOpen = bIsOpen;
	if (!m_bIsExporting)
		OnRenderSizeChanged();
}

void Viewports::setWindowSize(int width, int height) {
	m_Window.setSize(width, height);
}

void Viewports::setAvailableAppViewSize(int width, int height) {
	m_AvailableAppView.setSize(width, height);
	if (!m_bIsExporting && !m_bIsOutputWindowOpen)
		OnRenderSizeChanged();
}

void Viewports::setExportSize(int width, int height) {
	m_Export.setSize(width, height);
}

void Viewports::setOutputWindowSize(int width, int height) {
	m_OutputWindow.setSize(width, height);
	if (!m_bIsExporting && m_bIsOutputWindowOpen)
		OnRenderSizeChanged();
}

RectSizePos Viewports::AppView() {
	if (!m_bIsExporting && !m_bIsOutputWindowOpen && !m_bConstrainAppViewRatio)
		return m_AvailableAppView;
	else {
		// Get aspect ratios
		float appViewRatio = m_AvailableAppView.aspectRatio();
		float aspectRatio;
		if (m_bIsExporting)
			aspectRatio = m_Export.aspectRatio();
		else if (m_bIsOutputWindowOpen)
			aspectRatio = m_OutputWindow.aspectRatio();
		else
			aspectRatio = m_appViewConstrainedRatio;
		// Compute size
		RectSizePos res;
		if (aspectRatio > appViewRatio)
			res.setSize(m_AvailableAppView.width(), static_cast<int>(m_AvailableAppView.width() / aspectRatio));
		else
			res.setSize(static_cast<int>(m_AvailableAppView.height() * aspectRatio), m_AvailableAppView.height());
		// Compute position
		glm::ivec2 center = (m_AvailableAppView.topLeft() + m_AvailableAppView.botRight()) / 2;
		glm::ivec2 topLeft = center - res.size() / 2;
		res.setTopLeft(topLeft.x, topLeft.y);
		//
		return res;
	}
}

void Viewports::ImGuiConstrainAppViewRatio() {
	if (ImGui::Checkbox("Constrain aspect ratio", &m_bConstrainAppViewRatio)) {
		OnRenderSizeChanged();
	}
	if (m_bConstrainAppViewRatio) {
		if (ImGui::SliderFloat("Aspect ratio", &m_appViewConstrainedRatio, 0.5f, 2.0f)) {
			OnRenderSizeChanged();
		}
	}
}