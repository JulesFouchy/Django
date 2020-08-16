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
	const std::string& getSVGName(size_t svgIndex) const;
	const std::string& getLayoutName(size_t layoutIndex) const;
	inline size_t nbSVGs() { return m_svgPositionsInSsbo.size(); }

	void addSVGShape(const std::string& svgFilepath);
	void uploadAllSVGData();
	void createAllThumbnails(std::unordered_map<std::string, ActionBinding*>& svgActionBindings, ThumbnailFactory& thumbnailFactory);

	void setNbLayouts(size_t size);
	void pushLayout(const std::string& layoutName, const std::string& srcCode);

private:
	SSBO<float> m_ssbo;
	std::vector<float> m_data;
	size_t m_previousDataSize = 0;
	std::vector<SVGPositionInSsbo> m_svgPositionsInSsbo;
	std::vector<std::string> m_svgNames;

	std::vector<ConfigGPU> m_layoutConfigs;
};