#include "ConfigFromSVG.h"

#include <nanosvg/nanosvg.h>

void CreateConfigFromSVG(const std::string& filepath) {
	struct NSVGimage* image;
	image = nsvgParseFromFile(filepath.c_str(), "px", 96);
	printf("size: %f x %f\n", image->width, image->height);
	// Use...
	for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
		for (auto path = shape->paths; path != NULL; path = path->next) {
			for (int i = 0; i < path->npts - 1; i += 3) {
				float* p = &path->pts[i * 2];
				spdlog::info(p[0]);
				//drawCubicBez(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
			}
		}
	}
	// Delete
	nsvgDelete(image);
}