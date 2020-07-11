#include "ConfigManager.h"

#include <filesystem>
namespace fs = std::filesystem;
#include "Helper/File.h"

const std::string SHAPES_FOLDER = "configurations/shapes";
const std::string LAYOUTS_FOLDER = "configurations/layouts";

ConfigManager::ConfigManager() {
    // Get all shapes source code
    size_t nbShapesFiles = 0;
    for (const auto& entry : fs::directory_iterator(SHAPES_FOLDER))
        nbShapesFiles++;
    std::vector<std::string> shapesSrcCode;
    shapesSrcCode.reserve(nbShapesFiles);
    for (const auto& entry : fs::directory_iterator(SHAPES_FOLDER)) {
        std::string str;
        MyFile::ToString(entry.path().string(), &str);
        shapesSrcCode.push_back(str);
    }
    for (auto& t : shapesSrcCode) {
        spdlog::info(t);
        Log::separationLine();
    }
}