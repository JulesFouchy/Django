#pragma once

#include "OpenGL/RenderBuffer.h"
#include "Helper/CircularBuffer.h"

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

	void update(Renderer& renderer, std::unique_ptr<Clock>& clock);
	void ImGui();

private:
	unsigned int m_width  = 1280;
	unsigned int m_height = 720;
	float m_fps = 30.0f;
	float m_durationAfterLastAction = 10.0f;
	std::string m_exportFolderPath;
	std::string m_prefix = "";

	RenderBuffer m_renderBuffer;
	float m_timeExportStops;
	unsigned int m_frameCount;
	unsigned int m_totalNbFrames;
	int m_maxNbDigitsOfFrameCount;
	CircularBuffer<float, 50> m_framesTime;
	bool m_bIsExporting = false;
};