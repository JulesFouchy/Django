#pragma once

#include "RectSizePos.h"

class Viewports {
public:
	static RectSize RenderSize(); // Size of the image that should be rendered
	static RectSizePos AppView(); // Size of the actual viewing area and its position inside the window
	inline static glm::ivec2 SwapYConvention(const glm::ivec2& p) { return { p.x, m_Window.height() - p.y }; }

	static void setIsExporting(bool bIsExporting);
	static void setIsOutputWindowOpen(bool bIsOpen);
	static void setWindowSize          (int width, int height);
	static void setAvailableAppViewSize(int width, int height);
	static void setExportSize          (int width, int height);
	static void setOutputWindowSize    (int width, int height);

	static inline void setWindowTopLeft (int x, int y) { m_Window .setTopLeft(x, y); }
	static inline void setAvailableAppViewTopLeft(int x, int y) { m_AvailableAppView.setTopLeft(x, y); }

	static inline const glm::ivec2& getExportSize()           { return m_Export.size(); }
	static inline       glm::ivec2  getWindowTopLeft()        { return m_Window.topLeft(); }
	static inline const glm::ivec2& getWindowSize()           { return m_Window.size(); }
	static inline const glm::ivec2& getAvailableAppViewSize() { return m_AvailableAppView.size(); }
	static inline const glm::ivec2& getOutputWindowSize()     { return m_OutputWindow.size(); }
	static inline bool IsExporting() { return m_bIsExporting; }
	static inline bool IsOutputWindowOpen() { return m_bIsOutputWindowOpen; }

	static inline void setRenderSizeChangedCallback(std::function<void()> callback) { OnRenderSizeChanged = callback; }

	static void ImGuiConstrainAppViewRatio();

private:
	static std::function<void()> OnRenderSizeChanged;

private:
	static RectSizePos m_Window;           // Size of the window, and it's position inside the screen
	static RectSizePos m_AvailableAppView; // Size of the available viewing area (excludes the docked ImGui windows), and its position inside the window
	static RectSize m_Export;              // Size of the exported images
	static RectSize m_OutputWindow;        // Size of the second window, used as the final output that the spectators see

	static bool m_bIsExporting;        // Owned by Viewports because it needs to know it when deciding what the render size should be
	static bool m_bIsOutputWindowOpen; // Same as above

	static bool m_bConstrainAppViewRatio;
	static float m_appViewConstrainedRatio;
};