#pragma once

class SVGConfigFactory {
public:
	SVGConfigFactory() = delete;
	static void ImGui();

private:
	static void CreateConfigFromSVG(const std::string& filepath);

private:
	static std::string m_filepath;
};