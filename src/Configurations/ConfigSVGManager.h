#pragma once

#include "ConfigGPU.h"

#include "OpenGL/ComputeShader.h"
#include "OpenGL/SSBO.h"

class ThumbnailFactory;
struct ActionBinding;

struct SVGPositionInSsbo {
	int nbCurves;
	int offsetInSsbo;
};

class ConfigSVGManager {
public:
	ConfigSVGManager();
	~ConfigSVGManager() = default;

	Configuration& getConfig(size_t svgIndex, size_t layoutIndex);
	inline size_t nbSVGs() { return m_svgPositionsInSsbo.size(); }

	void addSVGShape(const std::string& svgFilepath);
	void uploadAllSVGData();
	void createAllThumbnails(std::unordered_map<std::string, ActionBinding*>& svgActionBindings, ThumbnailFactory& thumbnailFactory);

	void setNbLayouts(size_t size);
	void pushLayout(const std::string& srcCode);

private:
	SSBO m_ssbo;
	std::vector<float> m_data;
	size_t m_previousDataSize = 0;
	std::vector<SVGPositionInSsbo> m_svgPositionsInSsbo;

	std::vector<ConfigGPU> m_layoutConfigs;
};