#include "ConfigSVGManager.h"

#include "Actions/ThumbnailFactory.h"
#include "Actions/ActionBinding.h"
#include <nanosvg/nanosvg.h>

ConfigSVGManager::ConfigSVGManager()
	: m_ssbo(3, GL_STREAM_READ)
{}

Configuration& ConfigSVGManager::getConfig(size_t svgIndex, size_t layoutIndex) {
	ConfigGPU& config = m_layoutConfigs[layoutIndex];
	config.m_computeShader.get().bind();
	config.m_computeShader.get().setUniform1i("u_nbCurves", m_svgPositionsInSsbo[svgIndex].nbCurves);
	config.m_computeShader.get().setUniform1i("u_offsetInSsbo", m_svgPositionsInSsbo[svgIndex].offsetInSsbo);
	config.m_computeShader.get().unbind();
	return config;
}

void ConfigSVGManager::addSVGShape(const std::string& svgFilepath) {
	// Open SVG file
	struct NSVGimage* image;
	image = nsvgParseFromFile(svgFilepath.c_str(), "px", 96);
	if (!image) {
		spdlog::warn("[ConfigSVGManager::addSVGShape] Invalid file path : {}", svgFilepath);
	}
	// Get bezier curves
	for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
		for (auto path = shape->paths; path != NULL; path = path->next) {
			for (int i = 0; i < path->npts - 1; i += 3) {
				float* p = &path->pts[i * 2];
				m_data.push_back(p[0] / image->height * 2.0 - 1.0);
				m_data.push_back(1.0 - p[1] / image->height * 2.0);
				m_data.push_back(p[2] / image->height * 2.0 - 1.0);
				m_data.push_back(1.0 - p[3] / image->height * 2.0);
				m_data.push_back(p[4] / image->height * 2.0 - 1.0);
				m_data.push_back(1.0 - p[5] / image->height * 2.0);
				m_data.push_back(p[6] / image->height * 2.0 - 1.0);
				m_data.push_back(1.0 - p[7] / image->height * 2.0);
			}
		}
	}
	// Cleanup
	nsvgDelete(image);
	// Init SVG position in ssbo
	assert(((m_data.size() - m_previousDataSize) % 8 == 0) && "Problem while reading the SVG !");
	m_svgPositionsInSsbo.emplace_back();
	m_svgPositionsInSsbo.back().nbCurves = (m_data.size() - m_previousDataSize) / 8;
	m_svgPositionsInSsbo.back().offsetInSsbo = m_previousDataSize / 8;
	//
	m_previousDataSize = m_data.size();
}

void ConfigSVGManager::createAllThumbnails(std::unordered_map<std::string, ActionBinding*>& svgActionBindings, ThumbnailFactory& thumbnailFactory) {
	for (auto& kv : svgActionBindings) {
		size_t idx = kv.second->action.index;
		kv.second->action.thumbnailTextureID = thumbnailFactory.createTextureForSVG(
			m_svgPositionsInSsbo[idx].nbCurves,
			m_svgPositionsInSsbo[idx].offsetInSsbo
		);
	}
}

void ConfigSVGManager::uploadAllSVGData() {
	m_ssbo.uploadData(m_data.size(), m_data.data());
	std::vector<float>().swap(m_data); // deallocate memory
}

void ConfigSVGManager::setNbLayouts(size_t size) {
	m_layoutConfigs.reserve(size);
}

void ConfigSVGManager::pushLayout(const std::string& srcCode) {
	m_layoutConfigs.emplace_back();
	m_layoutConfigs.back().initWithSrcCode(srcCode);
}