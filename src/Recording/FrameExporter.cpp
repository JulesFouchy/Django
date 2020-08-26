#include "FrameExporter.h"

#include "Constants/FolderPath.h"
#include <stb_image/stb_image_write.h>
#include "Helper/String.h"
#include "Helper/File.h"
#include "Renderer.h"
#include "Record.h"

FrameExporter::FrameExporter()
	: m_exportFolderPath(FolderPath::Exports)
{}

void FrameExporter::startExporting(Record& selectedRecord, Renderer& renderer) {
	MyFile::CreateFolderIfDoesntExist(m_exportFolderPath);
	m_renderBuffer.setSize(m_width, m_height);
	renderer.attachRenderbuffer(m_renderBuffer);
	m_frameCount = 0;
	m_maxNbDigitsOfFrameCount = std::ceil(std::log10(selectedRecord.totalDuration() * m_fps));
	m_bIsExporting = true;
}

void FrameExporter::stopExporting(Renderer& renderer) {
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

void FrameExporter::ImGui(Record* selectedRecord, Renderer& renderer) {
	if (selectedRecord) {
		if (!m_bIsExporting) {
			if (ImGui::Button("Export"))
				startExporting(*selectedRecord, renderer);
		}
		else {
			if (ImGui::Button("Stop exporting"))
				stopExporting(renderer);
		}
	}
}