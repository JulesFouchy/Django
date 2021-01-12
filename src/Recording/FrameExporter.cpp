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
#include "Viewports/Viewports.h"
#include "OpenGL/RenderBuffer.h"

FrameExporter::FrameExporter()
	: m_exportFolderBasePath(FolderPath::Exports)
{
	Viewports::setExportSize(1280, 720);
}

void FrameExporter::startExporting(Record& selectedRecord, Renderer& renderer, std::unique_ptr<Clock>& clock, const glm::vec3& bgColor) {
	m_exportFolderPath = m_exportFolderBasePath + "/" + selectedRecord.name();
	if (MyFile::CreateFolderIfDoesntExist(m_exportFolderPath)) {
		Viewports::setIsExporting(true);
		renderer.clearRenderBuffer(bgColor);
		m_frameCount = 0;
		float totalExportDuration = selectedRecord.totalDuration();
		m_timeExportStops = selectedRecord.initialTime() + totalExportDuration;
		m_totalNbFrames = static_cast<unsigned int>(std::ceil(totalExportDuration * m_fps));
		m_maxNbDigitsOfFrameCount = static_cast<int>(std::ceil(std::log10(m_totalNbFrames)));
		clock = std::make_unique<Clock_FixedTimestep>(m_fps, selectedRecord.initialTime());
		m_frameAverageTime.clear();
		m_lastSDLCounter = SDL_GetPerformanceCounter();
	}
	else {
		spdlog::warn("[FrameExporter::startExporting] Couldn't start exporting because folder creation failed !");
	}
}

void FrameExporter::stopExporting(Renderer& renderer, std::unique_ptr<Clock>& clock) {
	clock = std::make_unique<Clock_Realtime>();
	Viewports::setIsExporting(false);
}

void FrameExporter::exportFrame(RenderBuffer& renderBuffer) {
	// Get pixel data
	renderBuffer.bind();
	int width = renderBuffer.width();
	int height = renderBuffer.height();
	unsigned char* data = new unsigned char[4 * width * height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	renderBuffer.unbind();
	// Write png
	stbi_flip_vertically_on_write(1);
	std::string filepath = m_exportFolderPath + "/" + m_prefix + MyString::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + ".png";
	stbi_write_png(filepath.c_str(), width, height, 4, data, 0);
	delete[] data;
	// Measure frame time
	m_frameAverageTime.push((SDL_GetPerformanceCounter() - m_lastSDLCounter) / (float)SDL_GetPerformanceFrequency());
	m_lastSDLCounter = SDL_GetPerformanceCounter();
	//
	m_frameCount++;
}

void FrameExporter::update(Renderer& renderer, std::unique_ptr<Clock>& clock) {
	if (Viewports::IsExporting()) {
		if (clock->time() > m_timeExportStops) {
			stopExporting(renderer, clock);
		}
	}
}

void FrameExporter::ImGui() {
	if (!Viewports::IsExporting()) {
		ImGui::Text("Resolution : "); ImGui::SameLine();
		ImGui::PushItemWidth(50);
		unsigned int w = static_cast<unsigned int>(Viewports::getExportSize().x);
		unsigned int h = static_cast<unsigned int>(Viewports::getExportSize().y);
		MyImGui::InputUInt("W", &w); ImGui::SameLine();
		MyImGui::InputUInt("H", &h);
		Viewports::setExportSize(static_cast<int>(w), static_cast<int>(h));
		ImGui::PopItemWidth();
		ImGui::InputFloat("FPS", &m_fps);
		ImGui::InputText("Export to", &m_exportFolderBasePath);
	}
	else {
		ImGui::Text("Remaining time :");
		ImGui::SameLine();
		MyImGui::TimeFormatedHMS(m_frameAverageTime.get() * (m_totalNbFrames - m_frameCount));
		ImGui::Text(("Exported frames " + MyString::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + " / " + std::to_string(m_totalNbFrames)).c_str());
	}
}