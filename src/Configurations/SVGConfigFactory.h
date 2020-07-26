#pragma once

class SVGConfigFactory {
public:
	SVGConfigFactory() = delete;
	static void ImGui();

private:
	static bool CreateConfigFromSVG(const std::string& filepath);

private:
	static std::string m_filepath;
};