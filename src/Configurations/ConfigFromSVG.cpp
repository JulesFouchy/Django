#include "ConfigFromSVG.h"

#include <nanosvg/nanosvg.h>
#include "Helper/ArrayStringified.h"

void CreateConfigFromSVG(const std::string& filepath) {
	struct NSVGimage* image;
	image = nsvgParseFromFile(filepath.c_str(), "px", 96);
	if (!image)
		spdlog::warn("[CreateConfigFromSVG] Invalid file path : '{}'", filepath);
	//printf("size: %f x %f\n", image->width, image->height);
	// Use...
	for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
		for (auto path = shape->paths; path != NULL; path = path->next) {
			for (int i = 0; i < path->npts - 1; i += 3) {
				float* p = &path->pts[i * 2];
				ArrayStringified bezier;
				bezier.push("vec2(" + std::to_string(p[0] / image->height * 2.0 - 1.0) + ", " + std::to_string(p[1] / image->height * 2.0 - 1.0) + ")");
				bezier.push("vec2(" + std::to_string(p[2] / image->height * 2.0 - 1.0) + ", " + std::to_string(p[3] / image->height * 2.0 - 1.0) + ")");
				bezier.push("vec2(" + std::to_string(p[4] / image->height * 2.0 - 1.0) + ", " + std::to_string(p[5] / image->height * 2.0 - 1.0) + ")");
				bezier.push("vec2(" + std::to_string(p[6] / image->height * 2.0 - 1.0) + ", " + std::to_string(p[7] / image->height * 2.0 - 1.0) + ")");
				bezier.close();
				spdlog::info(bezier.string());
				//drawCubicBez(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
			}
		}
	}
	// Delete
	nsvgDelete(image);
}