#pragma once

class SVGConfigFactory {
public:
	SVGConfigFactory() = delete;
	static void ImGui();

private:
	static bool CreateConfigFromSVG(const std::string& filepath);
	static void SaveShaderFile(const std::string& svgFilepath, const std::string& sourceCode);

private:
	static std::string m_filepath;
};