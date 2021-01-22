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
#include <Boxer/boxer.h>
#include <nfd.hpp>
#include "Constants/Textures.h"

FrameExporter::FrameExporter()
	: m_settings({ 60.f, 1280, 720, FolderPath::Exports })
{
	const std::string& path = FolderPath::LastSession_ExportSettings;
	if (MyFile::Exists(path)) {
		std::ifstream is(path);
		{
			try {
				cereal::JSONInputArchive archive(is);
				archive(
					CEREAL_NVP(m_settings)
				);
			}
			catch (std::exception e) {
				spdlog::warn("[FrameExporter::FrameExporter] Invalid lastSessionExportSettings.json file. Starting with default values instead. \n{}", e.what());
			}
		}
	}
	Viewports::setExportSize(m_settings.width, m_settings.height);
}

FrameExporter::~FrameExporter() {
	std::ofstream os(FolderPath::LastSession_ExportSettings);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_settings)
		);
	}
}

bool FrameExporter::startExporting(Record& selectedRecord, Renderer& renderer, std::unique_ptr<Clock>& clock) {
	m_exportFolderPath = m_settings.folderBasePath + "/" + selectedRecord.name();
	if (MyFile::Exists(m_exportFolderPath)) {
		boxer::Selection answer = boxer::show(
			("The record \"" + selectedRecord.name() + "\" has already been exported. Do you want to overwrite the previously exported frames at \"" +
				m_exportFolderPath + "\" ?").c_str(),
			"Overwrite",
			boxer::Style::Question,
			boxer::Buttons::YesNo
		);
		// Cancel if user doesnt want to overwrite previous export
		if (answer != boxer::Selection::Yes)
			return false;
		else {
			try {
				std::filesystem::remove_all(m_exportFolderPath);
			}
			catch (std::exception e) {
				spdlog::warn("[FrameExporter::startExporting] Failed to clean the previously created frames.");
			}
		}
	}
	if (MyFile::CreateFolderIfDoesntExist(m_exportFolderPath)) {
		Viewports::setIsExporting(true);
		m_frameCount = 0;
		float totalExportDuration = selectedRecord.totalDuration();
		m_timeExportStops = selectedRecord.initialTime() + totalExportDuration;
		m_totalNbFrames = static_cast<unsigned int>(std::ceil(totalExportDuration * m_settings.fps));
		m_maxNbDigitsOfFrameCount = static_cast<int>(std::ceil(std::log10(m_totalNbFrames)));
		clock = std::make_unique<Clock_FixedTimestep>(m_settings.fps, selectedRecord.initialTime());
		m_frameAverageTime.clear();
		m_lastSDLCounter = SDL_GetPerformanceCounter();
		return true;
	}
	else {
		spdlog::warn("[FrameExporter::startExporting] Couldn't start exporting because folder creation failed !");
		return false;
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
	std::string filepath = m_exportFolderPath + "/" + MyString::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + ".png";
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
		m_settings.width = static_cast<int>(w);
		m_settings.height = static_cast<int>(h);
		Viewports::setExportSize(m_settings.width, m_settings.height);
		ImGui::PopItemWidth();
		ImGui::InputFloat("FPS", &m_settings.fps);
		ImGui::PushID(46248921652987);
		ImGui::InputText("", &m_settings.folderBasePath);
		ImGui::PopID(); ImGui::SameLine();
		if (MyImGui::ButtonWithIcon(Textures::Folder(), ImVec4(1, 1, 1, 1), ImVec4(0.1, 0.1, 0.1, 1), 1, 18, 18)) {
			NFD::UniquePath outPath;
			nfdresult_t result;
			if (MyFile::Exists(m_settings.folderBasePath))
				result = NFD::PickFolder(outPath, m_settings.folderBasePath.c_str());
			else
				result = NFD::PickFolder(outPath);
			if (result == NFD_OKAY) {
				m_settings.folderBasePath = outPath.get();
			}
		}
		ImGui::SameLine();
		ImGui::Text("Export to");
	}
	else {
		ImGui::Text("Remaining time :");
		ImGui::SameLine();
		MyImGui::TimeFormatedHMS(m_frameAverageTime.get() * (m_totalNbFrames - m_frameCount));
		ImGui::Text(("Exported frames " + MyString::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + " / " + std::to_string(m_totalNbFrames)).c_str());
	}
}