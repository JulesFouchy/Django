#pragma once

#include "OpenGL/RenderBuffer.h"

class FrameExporter {
public:
	FrameExporter();
	~FrameExporter() = default;

	void ImGui();
	inline bool isExporting() { return m_bIsExporting; }
	void getReadyForFrame();
	void exportFrame();

private:
	void startExporting(float recordDuration);
	void stopExporting();

private:
	int m_width  = 1920;
	int m_height = 1080;
	unsigned int m_fps = 30;
	std::string m_exportFolderPath;
	std::string m_prefix = "";

	RenderBuffer m_renderBuffer;
	unsigned int m_frameCount;
	int m_maxNbDigitsOfFrameCount;
	bool m_bIsExporting = false;
};