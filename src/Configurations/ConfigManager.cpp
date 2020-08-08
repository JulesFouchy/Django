#include "ConfigManager.h"

#include "Actions/ThumbnailFactory.h"

#include <filesystem>
namespace fs = std::filesystem;
#include "Helper/File.h"
#include "Helper/String.h"
#include "Helper/Input.h"
#include "Helper/Random.h"

constexpr float SCROLL_SPEED = 0.1f;

static const std::string VERSION = "#version 430";

static const std::string SHAPES_FOLDER       = "configurations/shapes";
static const std::string SVG_FOLDER          = "configurations/svgShapes";
static const std::string LAYOUTS_FOLDER      = "configurations/layouts";
static const std::string STANDALONE_FOLDER   = "configurations/standalones";
static const std::string SVG_TEMPLATE        = "internal-shaders/configTemplate_SVG.comp";
static const std::string STANDALONE_TEMPLATE = "internal-shaders/configTemplate_Layout_and_Standalone.comp";
static const std::string LAYOUT_TEMPLATE     = "internal-shaders/configTemplate_Layout_and_Standalone.comp";
static const std::string RANDOM_FILE         = "internal-shaders/random.glsl";

ConfigManager::ConfigManager() {
    ThumbnailFactory thumbnailFactory;
    // Get rand() source code
    std::string randSrc;
    MyFile::ToString(RANDOM_FILE, &randSrc);
    // Get all the template source codes
    std::string svgTemplate;
    std::string layoutTemplate;
    std::string standaloneTemplate;
    MyFile::ToString(SVG_TEMPLATE,        &svgTemplate);
    MyFile::ToString(LAYOUT_TEMPLATE,     &layoutTemplate);
    MyFile::ToString(STANDALONE_TEMPLATE, &standaloneTemplate);
    // Create all SVG shapes and set key bindings and thumbnails
    size_t i = 0;
    for (const auto& entry : fs::directory_iterator(SVG_FOLDER)) {
        m_svgManager.addSVGShape(entry.path().string());
        // key bindings
        m_keyBindings.addAction({
            "SVG " + MyString::FileName(entry.path().string()),
            (unsigned int)-1,
            ActionType::SVG_SHAPE,
            i++
        });
    }
    m_svgManager.uploadAllSVGData();
    // Get all shapes source code and set key bindings and thumbnail
    size_t nbShapesFiles = 0;
    for (const auto& entry : fs::directory_iterator(SHAPES_FOLDER)) {
        nbShapesFiles++;
    }
    std::vector<std::string> shapesSrcCode;
    shapesSrcCode.reserve(nbShapesFiles);
    i = 0;
    for (const auto& entry : fs::directory_iterator(SHAPES_FOLDER)) {
        // get source code
        std::string srcCode;
        MyFile::ToString(entry.path().string(), &srcCode);
        shapesSrcCode.push_back(srcCode);
        // thumbnail
        unsigned int texID = thumbnailFactory.createTexture(ActionType::SHAPE, srcCode);
        // key binding
        m_keyBindings.addAction({
            "Shape " + MyString::FileName(entry.path().string()),
            texID,
            ActionType::SHAPE,
            i
        });
        i++;
    }
    // Set Shape-Layout Array2D size and SVGShape-Layout size and set layout key bindings
    size_t nbLayoutsFiles = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER)) {
        nbLayoutsFiles++;
    }
    m_shapeLayoutConfigs.setSize(nbShapesFiles, nbLayoutsFiles);
    m_svgManager.setNbLayouts(nbLayoutsFiles);
    // Create all Shape-Layout and SVGShape-Layout shaders
    size_t y = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER)) {
        std::string layoutSrc;
        MyFile::ToString(entry.path().string(), &layoutSrc);
        // thumbnail
        unsigned int texID = thumbnailFactory.createTexture(ActionType::LAYOUT, randSrc + layoutSrc);
        // key binding
        m_keyBindings.addAction({
            "Layout " + MyString::FileName(entry.path().string()),
            texID,
            ActionType::LAYOUT,
            y
        });
        size_t x = 0;
        // Shapes
        for (const std::string& shapeSrc : shapesSrcCode) {
            m_shapeLayoutConfigs(x, y).initWithSrcCode(
                VERSION + "\n" +
                shapeSrc + "\n" +
                randSrc + "\n" +
                layoutSrc + "\n" +
                layoutTemplate
            );
            x++;
        }
        // SVG shapes
        m_svgManager.pushLayout(
            VERSION + "\n" +
            svgTemplate + "\n" +
            randSrc + "\n" +
            layoutSrc
        );
        y++;
    }
    // Create all standalone configurations and set key bindings
    size_t nbStandaloneConfigs = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER)) {
        nbStandaloneConfigs++;
    }
    m_standaloneConfigs.resize(nbStandaloneConfigs);
    i = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER)) {
        std::string src;
        MyFile::ToString(entry.path().string(), &src);
        // config
        m_standaloneConfigs[i].initWithSrcCode(
            VERSION + "\n" +
            randSrc + "\n" +
            src     + "\n" +
            standaloneTemplate
        );
        // thumbnail
        unsigned int texID = thumbnailFactory.createTexture(ActionType::STANDALONE, randSrc + src);
        // key binding
        m_keyBindings.addAction({
            MyString::FileName(entry.path().string()),
            texID,
            ActionType::STANDALONE,
            i
        });
        //
        i++;
    }
    //
    m_keyBindings.setupBindings();
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

void ConfigManager::applyAction(const Action& action) {
    switch (action.type)
    {
    case ActionType::SHAPE:
        m_currConfigType = ConfigType::SHAPE_LAYOUT;
        m_currShapeIndex = action.index;
        m_bLastShapeWasSVG = false;
        break;
    case ActionType::SVG_SHAPE:
        m_currConfigType = ConfigType::SVG_LAYOUT;
        m_currSvgIndex = action.index;
        m_bLastShapeWasSVG = true;
        break;
    case ActionType::LAYOUT:
        m_currConfigType = m_bLastShapeWasSVG ? ConfigType::SVG_LAYOUT : ConfigType::SHAPE_LAYOUT;
        m_currLayoutIndex = action.index;
        break;
    case ActionType::STANDALONE:
        m_currConfigType = ConfigType::STANDALONE;
        m_currStandaloneIndex = action.index;
        break;
    case ActionType::TEXT:
        if (m_currConfigType != ConfigType::TEXT) {
            m_currConfigType = ConfigType::TEXT;
            m_textConfig.setCaptureKeys(true);
        }
        else {
            m_textConfig.toggleCaptureKeys();
        }
        break;
    case ActionType::REROLL_RANDOM:
        m_randParams.seed = 10.0f * MyRand::_m1to1();
        break;
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
    if (m_currConfigType != ConfigType::TEXT || !m_textConfig.onKeyPressed(keyEvent)) {
        if (!m_params.onKeyPressed(keyEvent.keysym)) {
            const Action* action = m_keyBindings.getAction(scancode);
            if (action) {
                applyAction(*action);
                bHandled = true;
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