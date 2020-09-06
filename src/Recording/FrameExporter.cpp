#include "FrameExporter.h"

#include "Constants/FolderPath.h"
#include <stb_image/stb_image_write.h>
#include "Helper/String.h"
#include "Helper/File.h"
#include "Helper/MyImGui.h"
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
	float totalExportDuration = selectedRecord.totalDuration();
	m_timeExportStops = selectedRecord.initialTime() + totalExportDuration;
	m_totalNbFrames = std::ceil(totalExportDuration * m_fps);
	m_maxNbDigitsOfFrameCount = std::ceil(std::log10(m_totalNbFrames));
	clock = std::make_unique<Clock_FixedTimestep>(m_fps, selectedRecord.initialTime());
	m_bIsExporting = true;
	m_frameAverageTime.clear();
	m_lastSDLCounter = SDL_GetPerformanceCounter();
}

void FrameExporter::stopExporting(Renderer& renderer, std::unique_ptr<Clock>& clock) {
	clock = std::make_unique<Clock_Realtime>();
	renderer.detachRenderBuffer();
	m_bIsExporting = false;
}

void FrameExporter::exportFrame() {
	// Get pixel data
	m_renderBuffer.bind();
	unsigned char* data = new unsigned char[4 * m_width * m_height];
	glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	m_renderBuffer.unbind();
	// Write png
	stbi_flip_vertically_on_write(1);
	std::string filepath = m_exportFolderPath + "/" + m_prefix + MyString::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + ".png";
	stbi_write_png(filepath.c_str(), m_width, m_height, 4, data, 0);
	delete[] data;
	// Measure frame time
	m_frameAverageTime.push((SDL_GetPerformanceCounter() - m_lastSDLCounter) / (float)SDL_GetPerformanceFrequency());
	m_lastSDLCounter = SDL_GetPerformanceCounter();
	//
	m_frameCount++;
}

void FrameExporter::update(Renderer& renderer, std::unique_ptr<Clock>& clock) {
	if (isExporting()) {
		if (clock->time() > m_timeExportStops) {
			stopExporting(renderer, clock);
		}
	}
}

void FrameExporter::ImGui() {
	if (!isExporting()) {
		ImGui::Text("Resolution : "); ImGui::SameLine();
		ImGui::PushItemWidth(50);
		MyImGui::InputUInt("W", &m_width); ImGui::SameLine();
		MyImGui::InputUInt("H", &m_height);
		ImGui::PopItemWidth();
		ImGui::InputFloat("FPS", &m_fps);
		ImGui::InputText("Export to", &m_exportFolderPath);
	}
	else {
		ImGui::Text("Remaining time :");
		ImGui::SameLine();
		MyImGui::TimeFormatedHMS(m_frameAverageTime.get() * (m_totalNbFrames - m_frameCount));
		ImGui::Text(("Exported frames " + MyString::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + " / " + std::to_string(m_totalNbFrames)).c_str());
	}
}