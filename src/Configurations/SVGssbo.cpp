#include "SVGssbo.h"

#include <nanosvg/nanosvg.h>

int SVGssbo::s_id = 3;

SVGssbo::SVGssbo()
	: m_id(s_id++), m_ssbo(m_id, 8, GL_STREAM_READ)
{}

std::string SVGssbo::init(const std::string& svgFilepath) {
	// Open SVG file
	struct NSVGimage* image;
	image = nsvgParseFromFile(svgFilepath.c_str(), "px", 96);
	if (!image) {
		spdlog::warn("[SVGssbo::init] Invalid file path : {}", svgFilepath);
	}
	// Get bezier curves
	std::vector<float> data;
	for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
		for (auto path = shape->paths; path != NULL; path = path->next) {
			for (int i = 0; i < path->npts - 1; i += 3) {
				float* p = &path->pts[i * 2];
				data.push_back(p[0] / image->height * 2.0 - 1.0);
				data.push_back(1.0 - p[1] / image->height * 2.0);
				data.push_back(p[2] / image->height * 2.0 - 1.0);
				data.push_back(1.0 - p[3] / image->height * 2.0);
				data.push_back(p[4] / image->height * 2.0 - 1.0);
				data.push_back(1.0 - p[5] / image->height * 2.0);
				data.push_back(p[6] / image->height * 2.0 - 1.0);
				data.push_back(1.0 - p[7] / image->height * 2.0);
			}
		}
	}
	// Cleanup
	nsvgDelete(image);
	//
	m_ssbo.uploadData(data.size(), data.data());
	//
	return "struct Bezier { vec2 p1; vec2 p2; vec2 p3; vec2 p4; }; \n"
	"const uint nbCurves = " + std::to_string(data.size() / 8) +"; \n"
	"layout(std430, binding = " + std::to_string(m_id) + ") restrict readonly buffer bezier { \n"
	"	Bezier curves[]; \n"
	"}; \n";
}