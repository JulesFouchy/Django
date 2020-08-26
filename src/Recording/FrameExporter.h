#pragma once

#include "OpenGL/RenderBuffer.h"

class Record;
class Renderer;

class FrameExporter {
public:
	FrameExporter();
	~FrameExporter() = default;

	inline bool isExporting() { return m_bIsExporting; }
	void exportFrame();
	void ImGui(Record* selectedRecord, Renderer& renderer);

private:
	void startExporting(Record& selectedRecord, Renderer& renderer);
	void stopExporting(Renderer& renderer);

private:
	int m_width  = 100;
	int m_height = 100;
	unsigned int m_fps = 30;
	std::string m_exportFolderPath;
	std::string m_prefix = "";

	RenderBuffer m_renderBuffer;
	unsigned int m_frameCount;
	int m_maxNbDigitsOfFrameCount;
	bool m_bIsExporting = false;
};