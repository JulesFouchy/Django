#include "ConfigManager.h"

#include <filesystem>
namespace fs = std::filesystem;
#include "Helper/File.h"
#include "Helper/Input.h"

constexpr float SCROLL_SPEED = 0.1f;

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
    bool bHandled = false;
    // Count
    if (scancode == SDL_SCANCODE_KP_MINUS) {
        m_params.count--;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_PLUS) {
        m_params.count++;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_0 || scancode == SDL_SCANCODE_0) {
        m_params.count = 0;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_1 || scancode == SDL_SCANCODE_1) {
        m_params.count = 1;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_2 || scancode == SDL_SCANCODE_2) {
        m_params.count = 2;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_3 || scancode == SDL_SCANCODE_3) {
        m_params.count = 4;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_4 || scancode == SDL_SCANCODE_4) {
        m_params.count = 8;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_5 || scancode == SDL_SCANCODE_5) {
        m_params.count = 16;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_6 || scancode == SDL_SCANCODE_6) {
        m_params.count = 32;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_7 || scancode == SDL_SCANCODE_7) {
        m_params.count = 64;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_8 || scancode == SDL_SCANCODE_8) {
        m_params.count = 128;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_KP_9 || scancode == SDL_SCANCODE_9) {
        m_params.count = 256;
        bHandled = true;
    }
    // LR
    else if (scancode == SDL_SCANCODE_LEFT) {
        m_params.intLR--;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_RIGHT) {
        m_params.intLR++;
        bHandled = true;
    }
    // UD
    else if (scancode == SDL_SCANCODE_DOWN) {
        m_params.intUD--;
        bHandled = true;
    }
    else if (scancode == SDL_SCANCODE_UP) {
        m_params.intUD++;
        bHandled = true;
    }
    //
    if (m_shapeLayoutConfigs.getWidth() > 0) {
        if (scancode == SDL_SCANCODE_A) {
            m_currShapeIndex = (m_currShapeIndex - 1 + m_shapeLayoutConfigs.getWidth()) % m_shapeLayoutConfigs.getWidth();
            m_currConfigType = ConfigType::SHAPE_LAYOUT;
            bHandled = true;
        }
        if (scancode == SDL_SCANCODE_D) {
            m_currShapeIndex = (m_currShapeIndex + 1 + m_shapeLayoutConfigs.getWidth()) % m_shapeLayoutConfigs.getWidth();
            m_currConfigType = ConfigType::SHAPE_LAYOUT;
            bHandled = true;
        }
    }
    if (m_shapeLayoutConfigs.getHeight() > 0) {
        if (scancode == SDL_SCANCODE_S) {
            m_currLayoutIndex = (m_currLayoutIndex - 1 + m_shapeLayoutConfigs.getHeight()) % m_shapeLayoutConfigs.getHeight();
            m_currConfigType = ConfigType::SHAPE_LAYOUT;
            bHandled = true;
        }
        if (scancode == SDL_SCANCODE_W) {
            m_currLayoutIndex = (m_currLayoutIndex + 1 + m_shapeLayoutConfigs.getHeight()) % m_shapeLayoutConfigs.getHeight();
            m_currConfigType = ConfigType::SHAPE_LAYOUT;
            bHandled = true;
        }
    }    
    if (m_standaloneConfigs.size() > 0) {
        if (scancode == SDL_SCANCODE_Q) {
            m_currStandaloneIndex = (m_currStandaloneIndex - 1 + m_standaloneConfigs.size()) % m_standaloneConfigs.size();
            m_currConfigType = ConfigType::STANDALONE;
            bHandled = true;
        }
        if (scancode == SDL_SCANCODE_E) {
            m_currStandaloneIndex = (m_currStandaloneIndex + 1 + m_standaloneConfigs.size()) % m_standaloneConfigs.size();
            m_currConfigType = ConfigType::STANDALONE;
            bHandled = true;
        }
    }
    if (bHandled)
        applyTo(partSystem);
}

void ConfigManager::onWheel(float delta, ParticlesSystem& partSystem, bool bNoStandardScroll) {
    bool b = false;
    if (Input::KeyIsDown(SDL_SCANCODE_LCTRL) || Input::KeyIsDown(SDL_SCANCODE_RCTRL)) {
        m_params.ctrlWheel += delta * SCROLL_SPEED;
        b = true;
    }
    if (Input::KeyIsDown(SDL_SCANCODE_LSHIFT) || Input::KeyIsDown(SDL_SCANCODE_RSHIFT)) {
        m_params.shiftWheel += delta * SCROLL_SPEED;
        b = true;
    }
    if (Input::KeyIsDown(SDL_SCANCODE_LALT) || Input::KeyIsDown(SDL_SCANCODE_RALT)) {
        m_params.altWheel += delta * SCROLL_SPEED;
        b = true;
    }
    if (!b && !bNoStandardScroll) {
        m_params.wheel += delta * SCROLL_SPEED;
    }
    applyTo(partSystem);
}

Configuration& ConfigManager::get() {
    switch (m_currConfigType)
    {
    case ConfigType::SHAPE_LAYOUT:
        return m_shapeLayoutConfigs(m_currShapeIndex, m_currLayoutIndex);
    case ConfigType::STANDALONE:
        return m_standaloneConfigs[m_currStandaloneIndex];
    default:
        break;
    }
}