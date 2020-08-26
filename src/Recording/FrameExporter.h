#pragma once

#include "OpenGL/RenderBuffer.h"

class Record;
class Renderer;
class Clock;

class FrameExporter {
public:
	FrameExporter();
	~FrameExporter() = default;

	inline bool isExporting() { return m_bIsExporting; }
	void exportFrame();

	void startExporting(Record& selectedRecord, Renderer& renderer, std::unique_ptr<Clock>& clock, const glm::vec3& bgColor);
	void stopExporting(Renderer& renderer, std::unique_ptr<Clock>& clock);

private:
	int m_width  = 600;
	int m_height = 600;
	float m_fps = 30.0f;
	std::string m_exportFolderPath;
	std::string m_prefix = "";

	RenderBuffer m_renderBuffer;
	unsigned int m_frameCount;
	int m_maxNbDigitsOfFrameCount;
	bool m_bIsExporting = false;
};