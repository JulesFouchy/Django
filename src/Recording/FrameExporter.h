#pragma once

#include "OpenGL/RenderBuffer.h"

class Record;
class AlphaTrailSettings;

class FrameExporter {
public:
	FrameExporter();
	~FrameExporter() = default;

	inline bool isExporting() { return m_bIsExporting; }
	void exportFrame();
	void ImGui(Record* selectedRecord, AlphaTrailSettings& renderer);

private:
	void startExporting(Record& selectedRecord, AlphaTrailSettings& renderer);
	void stopExporting(AlphaTrailSettings& renderer);

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