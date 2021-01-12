#pragma once

#include "Helper/Averager.h"

class RenderBuffer;
class Record;
class Renderer;
class Clock;

struct FrameExporterSettings {
	float fps;
	std::string exportFolderBasePath;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(fps),
			CEREAL_NVP(exportFolderBasePath)
		);
	}
};

class FrameExporter {
public:
	FrameExporter();
	~FrameExporter();

	void exportFrame(RenderBuffer& renderBuffer);

	/// Returns true iff the export started successfully
	bool startExporting(Record& selectedRecord, Renderer& renderer, std::unique_ptr<Clock>& clock, const glm::vec3& bgColor);
	void stopExporting(Renderer& renderer, std::unique_ptr<Clock>& clock);

	void update(Renderer& renderer, std::unique_ptr<Clock>& clock);
	void ImGui();

private:
	FrameExporterSettings m_settings;
	std::string m_exportFolderPath;

	float m_timeExportStops;
	unsigned int m_frameCount;
	unsigned int m_totalNbFrames;
	int m_maxNbDigitsOfFrameCount;
	Averager<float> m_frameAverageTime;
	Uint64 m_lastSDLCounter;
};