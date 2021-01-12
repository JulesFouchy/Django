#pragma once

#include "Helper/Averager.h"

class RenderBuffer;
class Record;
class Renderer;
class Clock;

class FrameExporter {
public:
	FrameExporter();
	~FrameExporter() = default;

	void exportFrame(RenderBuffer& renderBuffer);

	/// Returns true iff the export started successfully
	bool startExporting(Record& selectedRecord, Renderer& renderer, std::unique_ptr<Clock>& clock, const glm::vec3& bgColor);
	void stopExporting(Renderer& renderer, std::unique_ptr<Clock>& clock);

	void update(Renderer& renderer, std::unique_ptr<Clock>& clock);
	void ImGui();

private:
	float m_fps = 60.0f;
	std::string m_exportFolderBasePath;
	std::string m_exportFolderPath;
	std::string m_prefix = "";

	float m_timeExportStops;
	unsigned int m_frameCount;
	unsigned int m_totalNbFrames;
	int m_maxNbDigitsOfFrameCount;
	Averager<float> m_frameAverageTime;
	Uint64 m_lastSDLCounter;
};