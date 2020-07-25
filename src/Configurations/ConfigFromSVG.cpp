#include "ConfigFromSVG.h"

#include <nanosvg/nanosvg.h>
#include "Helper/ArrayStringified.h"
#include "Helper/File.h"

void CreateConfigFromSVG(const std::string& filepath) {
	// Get base source code
	std::string srcCode;
	MyFile::ToString(MyFile::RootDir + "/internal-shaders/bezierSplineTemplate.comp", &srcCode);
	// Open SVG file
	struct NSVGimage* image;
	image = nsvgParseFromFile(filepath.c_str(), "px", 96);
	if (!image)
		spdlog::warn("[CreateConfigFromSVG] Invalid file path : '{}'", filepath);
	// Get bezier curves
	ArrayStringified curves;
	unsigned int nbCurves = 0;
	for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
		for (auto path = shape->paths; path != NULL; path = path->next) {
			for (int i = 0; i < path->npts - 1; i += 3) {
				float* p = &path->pts[i * 2];
				ArrayStringified bezier;
				bezier.push("vec2(" + std::to_string(p[0] / image->height * 2.0 - 1.0) + ", " + std::to_string(1.0 - p[1] / image->height * 2.0) + ")");
				bezier.push("vec2(" + std::to_string(p[2] / image->height * 2.0 - 1.0) + ", " + std::to_string(1.0 - p[3] / image->height * 2.0) + ")");
				bezier.push("vec2(" + std::to_string(p[4] / image->height * 2.0 - 1.0) + ", " + std::to_string(1.0 - p[5] / image->height * 2.0) + ")");
				bezier.push("vec2(" + std::to_string(p[6] / image->height * 2.0 - 1.0) + ", " + std::to_string(1.0 - p[7] / image->height * 2.0) + ")");
				bezier.close();
				curves.push(bezier.string());
				nbCurves++;
			}
		}
	}
	curves.close();
	// Cleanup
	nsvgDelete(image);
	// Create shader
	srcCode =
		"struct Bezier {vec2 p1; vec2 p2; vec2 p3; vec2 p4;}; \n"
		"const uint nbCurves = " + std::to_string(nbCurves) + "; \n"
		"Bezier curves[nbCurves] = " + curves.string() + "; \n"
		+ srcCode;
	spdlog::info(srcCode);
}