#include "ConfigManager.h"

#include <filesystem>
namespace fs = std::filesystem;
#include "Helper/File.h"
#include "Helper/Input.h"
#include "Helper/Random.h"

constexpr float SCROLL_SPEED = 0.1f;

const std::string VERSION = "#version 430";

const std::string SHAPES_FOLDER = "configurations/shapes";
const std::string SVG_FOLDER = "configurations/svgShapes";
const std::string SVGSHAPE_FILE = "internal-shaders/svgShape.comp";
const std::string LAYOUTS_FOLDER = "configurations/layouts";
const std::string STANDALONE_FOLDER = "configurations/standalones";
const std::string RANDOM_FILE = "internal-shaders/random.glsl";

ConfigManager::ConfigManager() {
    // Get rand() source code
    std::string randSrc;
    MyFile::ToString(RANDOM_FILE, &randSrc);
    // Get SVG Shape source code
    std::string svgSrc;
    MyFile::ToString(SVGSHAPE_FILE, &svgSrc);
    // Create all SVG shapes
    for (const auto& entry : fs::directory_iterator(SVG_FOLDER)) {
        m_svgManager.addSVGShape(entry.path().string());
    }
    m_svgManager.uploadAllSVGData();
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
    // Set Shape-Layout Array2D size and SVGShape-Layout size
    size_t nbLayoutsFiles = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER))
        nbLayoutsFiles++;
    m_shapeLayoutConfigs.setSize(nbShapesFiles, nbLayoutsFiles);
    m_svgManager.setNbLayouts(nbLayoutsFiles);
    // Create all Shape-Layout and SVGShape-Layout shaders
    size_t y = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER)) {
        std::string layoutSrc;
        MyFile::ToString(entry.path().string(), &layoutSrc);
        size_t x = 0;
        // Shapes
        for (const std::string& shapeSrc : shapesSrcCode) {
            m_shapeLayoutConfigs(x, y).initWithSrcCode(
                 VERSION  + "\n" +
                 shapeSrc + "\n" +
                 randSrc  + "\n" +
                 layoutSrc
            );
            x++;
        }
        // SVG shapes
        m_svgManager.pushLayout(
            VERSION + "\n" +
            svgSrc + "\n" +
            randSrc + "\n" +
            layoutSrc
        );
        y++;
    }
    // Create all standalone configurations
    size_t nbStandaloneConfigs = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER))
        nbStandaloneConfigs++;
    m_standaloneConfigs.resize(nbStandaloneConfigs);
    size_t i = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER)) {
        std::string src;
        MyFile::ToString(entry.path().string(), &src);
        m_standaloneConfigs[i].initWithSrcCode(
            VERSION + "\n" +
            randSrc + "\n" +
            src
        );
        i++;
    }
}

Configuration& ConfigManager::get() {
    switch (m_currConfigType)
    {
    case ConfigType::SHAPE_LAYOUT:
        return m_shapeLayoutConfigs(m_currShapeIndex, m_currLayoutIndex);
    case ConfigType::SVG_LAYOUT:
        return m_svgManager.getConfig(m_currSvgIndex, m_currLayoutIndex);
    case ConfigType::STANDALONE:
        return m_standaloneConfigs[m_currStandaloneIndex];
    case ConfigType::TEXT:
        return m_textConfig;
    default:
        break;
    }
}

void ConfigManager::Imgui(ParticlesSystem& partSystem) {
    ImGui::Begin("Random");
    if (ImGui::DragFloat("Seed", &m_randParams.seed))
        applyTo(partSystem);
    if (ImGui::DragFloat2("X/Y Seed", &m_randParams.xySeed[0]))
        applyTo(partSystem);
    ImGui::End();
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

void ConfigManager::onKeyPressed(SDL_KeyboardEvent keyEvent, ParticlesSystem& partSystem) {
    SDL_Scancode scancode = keyEvent.keysym.scancode;
    bool bHandled = false;
    // Text Configuration
    if (scancode == SDL_SCANCODE_RETURN || scancode == SDL_SCANCODE_KP_ENTER) {
        if (m_currConfigType != ConfigType::TEXT) {
            m_currConfigType = ConfigType::TEXT;
            m_textConfig.setCaptureKeys(true);
        }
        else {
            m_textConfig.toggleCaptureKeys();
        }
        bHandled = true;
    }
    //
    if (m_currConfigType != ConfigType::TEXT || !m_textConfig.onKeyPressed(keyEvent)) {
        if (!m_params.onKeyPressed(keyEvent.keysym)) {
            // Random seed
            if (scancode == SDL_SCANCODE_SPACE) {
                m_randParams.seed = 10.0f * MyRand::_m1to1();
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
            if (scancode == SDL_SCANCODE_Z) {
                m_currSvgIndex = (m_currSvgIndex - 1 + m_svgManager.nbSVGs()) % m_svgManager.nbSVGs();
                m_currConfigType = ConfigType::SVG_LAYOUT;
                bHandled = true;
            }
            if (scancode == SDL_SCANCODE_X) {
                m_currSvgIndex = (m_currSvgIndex + 1 + m_svgManager.nbSVGs()) % m_svgManager.nbSVGs();
                m_currConfigType = ConfigType::SVG_LAYOUT;
                bHandled = true;
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
        }
        else {
            bHandled = true;
        }
    }
    else {
        bHandled = true;
    }
    if (bHandled)
        applyTo(partSystem);
}