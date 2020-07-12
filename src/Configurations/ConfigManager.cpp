#include "ConfigManager.h"

#include <filesystem>
namespace fs = std::filesystem;
#include "Helper/File.h"

const std::string SHAPES_FOLDER = "configurations/shapes";
const std::string LAYOUTS_FOLDER = "configurations/layouts";
const std::string STANDALONE_FOLDER = "configurations/standalones";

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
    // Set Array2D size
    size_t nbLayoutsFiles = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER))
        nbLayoutsFiles++;
    m_shapeLayoutConfigs.setSize(nbShapesFiles, nbLayoutsFiles);
    // Create all shape-layout shaders
    size_t y = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER)) {
        std::string layoutSrc;
        MyFile::ToString(entry.path().string(), &layoutSrc);
        size_t x = 0;
        for (const std::string& shapeSrc : shapesSrcCode) {
            m_shapeLayoutConfigs(x, y).initWithSrcCode(
                "#version 430\n" +
                 shapeSrc + "\n" +
                 layoutSrc
            );
            x++;
        }
        y++;
    }
    // Create all standalone configurations
    size_t nbStandaloneConfigs = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER))
        nbStandaloneConfigs++;
    m_standaloneConfigs.resize(nbStandaloneConfigs);
    size_t i = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER)) {
        m_standaloneConfigs[i].initWithFilePath(entry.path().string());
        i++;
    }
}

void ConfigManager::onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem) {
    if (m_shapeLayoutConfigs.getWidth() > 1) {
        if (scancode == SDL_SCANCODE_LEFT)
            m_currShapeIndex = (m_currShapeIndex   - 1) % m_shapeLayoutConfigs.getWidth();
        if (scancode == SDL_SCANCODE_RIGHT)
            m_currShapeIndex = (m_currShapeIndex   + 1) % m_shapeLayoutConfigs.getWidth();
    }
    if (m_shapeLayoutConfigs.getHeight() > 1) {
        if (scancode == SDL_SCANCODE_DOWN)
            m_currLayoutIndex = (m_currLayoutIndex - 1) % m_shapeLayoutConfigs.getHeight();
        if (scancode == SDL_SCANCODE_UP)
            m_currLayoutIndex = (m_currLayoutIndex + 1) % m_shapeLayoutConfigs.getHeight();
    }
    get().applyTo(partSystem);
}