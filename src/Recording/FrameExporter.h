#pragma once

#include "OpenGL/RenderBuffer.h"
#include "Helper/Averager.h"

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
	float m_fps = 60.0f;
	std::string m_exportFolderPath;
	std::string m_prefix = "";

	RenderBuffer m_renderBuffer;
	float m_timeExportStops;
	unsigned int m_frameCount;
	unsigned int m_totalNbFrames;
	int m_maxNbDigitsOfFrameCount;
	Averager<float> m_frameAverageTime;
	Uint64 m_lastSDLCounter;
	bool m_bIsExporting = false;
};