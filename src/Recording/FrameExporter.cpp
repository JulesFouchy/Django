#include "FrameExporter.h"

#include "Constants/FolderPath.h"
#include <stb_image/stb_image_write.h>
#include "Helper/String.h"
#include "Helper/File.h"
#include "Renderer.h"
#include "Record.h"
#include "Clock/Clock_FixedTimestep.h"
#include "Clock/Clock_Realtime.h"

FrameExporter::FrameExporter()
	: m_exportFolderPath(FolderPath::Exports)
{}

void FrameExporter::startExporting(Record& selectedRecord, Renderer& renderer, std::unique_ptr<Clock>& clock, const glm::vec3& bgColor) {
	MyFile::CreateFolderIfDoesntExist(m_exportFolderPath);
	m_renderBuffer.setSize(m_width, m_height);
	renderer.attachRenderbuffer(m_renderBuffer, bgColor);
	m_frameCount = 0;
	float totalExportDuration = selectedRecord.totalDuration() + m_durationAfterLastAction;
	m_timeExportStops = selectedRecord.initialTime() + totalExportDuration;
	m_maxNbDigitsOfFrameCount = std::ceil(std::log10(totalExportDuration * m_fps));
	clock = std::make_unique<Clock_FixedTimestep>(m_fps, selectedRecord.initialTime());
	m_bIsExporting = true;
}

void FrameExporter::stopExporting(Renderer& renderer, std::unique_ptr<Clock>& clock) {
	clock = std::make_unique<Clock_Realtime>();
	renderer.detachRenderBuffer();
	m_bIsExporting = false;
}

void FrameExporter::exportFrame() {
	m_renderBuffer.bind();
	unsigned char* data = new unsigned char[4 * m_width * m_height];
	glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(1);
	std::string filepath = m_exportFolderPath + "/" + m_prefix + MyString::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + ".png";
	stbi_write_png(filepath.c_str(), m_width, m_height, 4, data, 0);
	delete[] data;
	m_renderBuffer.unbind();
	m_frameCount++;
}

bool FrameExporter::update(Renderer& renderer, std::unique_ptr<Clock>& clock) {
	if (isExporting()) {
		if (clock->time() > m_timeExportStops) {
			stopExporting(renderer, clock);
			return true;
		}
	}
	return false;
}