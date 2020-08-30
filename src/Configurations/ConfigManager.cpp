#include "ConfigManager.h"

#include "Actions/ThumbnailFactory.h"
#include "Recording/RecordManager.h"
#include "Recording/StateChange.h"

#include <filesystem>
namespace fs = std::filesystem;
#include "Helper/File.h"
#include "Helper/String.h"
#include "Helper/Input.h"
#include "Helper/Random.h"
#include "Constants/FolderPath.h"
#include "StateModifier.h"

constexpr float SCROLL_SPEED = 0.1f;

static const std::string VERSION = "#version 430";

static const std::string SHAPES_FOLDER       = "configurations/shapes";
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

    // ----------------------------------
    //     Shapes (.svg and .comp)
    // ----------------------------------

    size_t nbShapesFiles = 0;
    for (const auto& entry : fs::directory_iterator(SHAPES_FOLDER)) {
        if (!MyString::FileExtension(entry.path().string()).compare(".comp"))
            nbShapesFiles++;
    }
    std::vector<std::string> shapesSrcCode;
    std::vector<std::string> shapesName;
    shapesSrcCode.reserve(nbShapesFiles);
    shapesName.reserve(nbShapesFiles);
    size_t   svgIdx = 0;
    size_t shapeIdx = 0;
    for (const auto& entry : fs::directory_iterator(SHAPES_FOLDER)) {
        const std::string fileExtension = MyString::FileExtension(entry.path().string());
        // SVG Shapes
        if (!fileExtension.compare(".svg")) {
            // add
            m_svgManager.addSVGShape(entry.path().string());
            // key binding
            m_keyBindings.addAction({
                MyString::FileName(entry.path().string()),
                (unsigned int)-1,
                ActionType::SVG_SHAPE,
                svgIdx++
            });
        }
        // Shapes .comp
        else if (!fileExtension.compare(".comp")) {
            // get source code
            std::string srcCode;
            MyFile::ToString(entry.path().string(), &srcCode);
            shapesSrcCode.push_back(srcCode);
            // name
            shapesName.push_back(MyString::FileName(entry.path().string()));
            // thumbnail
            unsigned int texID = thumbnailFactory.createTexture(ActionType::SHAPE, srcCode);
            // key binding
            m_keyBindings.addAction({
                MyString::FileName(entry.path().string()),
                texID,
                ActionType::SHAPE,
                shapeIdx++
            });
        }
        else {
            spdlog::warn("Unknown extension for a shape file : '{}'. Can only handle '.svg' and '.comp' !", fileExtension);
        }
    }
    // Finish SVG handling
    m_svgManager.uploadAllSVGData();
    m_svgManager.createAllThumbnails(m_keyBindings.getAllActionBindingsOfType(ActionType::SVG_SHAPE), thumbnailFactory);

    // ----------------------------------
    //             Layouts
    // ----------------------------------

    size_t nbLayoutsFiles = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER)) {
        if (!MyString::FileExtension(entry.path().string()).compare(".comp"))
            nbLayoutsFiles++;
    }
    m_shapeLayoutConfigs.setSize(nbShapesFiles, nbLayoutsFiles);
    m_svgManager.setNbLayouts(nbLayoutsFiles);
    size_t y = 0;
    for (const auto& entry : fs::directory_iterator(LAYOUTS_FOLDER)) {
        const std::string fileExtension = MyString::FileExtension(entry.path().string());
        if (!fileExtension.compare(".comp")) {
            const std::string layoutName = MyString::FileName(entry.path().string());
            // get source code
            std::string layoutSrc;
            MyFile::ToString(entry.path().string(), &layoutSrc);
            // thumbnail
            unsigned int texID = thumbnailFactory.createTexture(ActionType::LAYOUT, randSrc + layoutSrc);
            // key binding
            m_keyBindings.addAction({
                MyString::FileName(entry.path().string()),
                texID,
                ActionType::LAYOUT,
                y
            });
            size_t x = 0;
            // Shapes
            for (size_t k = 0; k < shapesSrcCode.size(); ++k) {
                m_shapeLayoutConfigs(x, y).setName(shapesName[k]);
                m_shapeLayoutConfigs(x, y).setLayoutName(layoutName);
                m_shapeLayoutConfigs(x, y).initWithSrcCode(
                    VERSION + "\n" +
                    shapesSrcCode[k] + "\n" +
                    randSrc + "\n" +
                    layoutSrc + "\n" +
                    layoutTemplate
                );
                x++;
            }
            // SVG shapes
            m_svgManager.pushLayout(
                layoutName,
                VERSION + "\n" +
                svgTemplate + "\n" +
                randSrc + "\n" +
                layoutSrc + "\n" +
                layoutTemplate
            );
            y++;
        }
        else {
            spdlog::warn("Unknown extension for a layout file : '{}'. Can only handle '.comp' !", fileExtension);
        }
    }

    // ----------------------------------
    //             Standalone
    // ----------------------------------

    size_t nbStandaloneConfigs = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER)) {
        if (!MyString::FileExtension(entry.path().string()).compare(".comp"))
            nbStandaloneConfigs++;
    }
    m_standaloneConfigs.resize(nbStandaloneConfigs);
    size_t i = 0;
    for (const auto& entry : fs::directory_iterator(STANDALONE_FOLDER)) {
        const std::string fileExtension = MyString::FileExtension(entry.path().string());
        const std::string fileName = MyString::FileName(entry.path().string());
        if (!fileExtension.compare(".comp")) {
            // create from source code
            std::string src;
            MyFile::ToString(entry.path().string(), &src);
            m_standaloneConfigs[i].initWithSrcCode(
                VERSION + "\n" +
                randSrc + "\n" +
                src     + "\n" +
                standaloneTemplate
            );
            // name
            m_standaloneConfigs[i].setName(fileName);
            // thumbnail
            unsigned int texID = thumbnailFactory.createTexture(ActionType::STANDALONE, randSrc + src);
            // key binding
            m_keyBindings.addAction({
                fileName,
                texID,
                ActionType::STANDALONE,
                i++
            });
        }
        else {
            spdlog::warn("Unknown extension for a standalone config file : '{}'. Can only handle '.comp' !", fileExtension);
        }
    }

    // Setup Bindings
    m_keyBindings.setupBindings(FolderPath::Settings + "/lastSessionBindings.json");
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

void ConfigManager::setCurrentConfigAsText() {
    if (m_currConfigType != ConfigType::TEXT) {
        m_currConfigType = ConfigType::TEXT;
        m_textConfig.setCaptureKeys(true);
    }
    else {
        m_textConfig.toggleCaptureKeys();
    }
}

void ConfigManager::Imgui(StateModifier& stateModifier) {
    ImGui::Begin("Random");
    m_randParams.ImGui(stateModifier);
    ImGui::End();
}

void ConfigManager::onWheel(float delta, bool bNoStandardScroll, StateModifier& stateModifier) {
    if (Input::KeyIsDown(SDL_SCANCODE_LCTRL) || Input::KeyIsDown(SDL_SCANCODE_RCTRL)) {
        m_params.setApplyAndRecord_CtrlWheel(m_params.getCtrlWheel() + delta * SCROLL_SPEED, stateModifier);
    }
    else if (Input::KeyIsDown(SDL_SCANCODE_LSHIFT) || Input::KeyIsDown(SDL_SCANCODE_RSHIFT)) {
        m_params.setApplyAndRecord_ShiftWheel(m_params.getShiftWheel() + delta * SCROLL_SPEED, stateModifier);
    }
    else if (Input::KeyIsDown(SDL_SCANCODE_LALT) || Input::KeyIsDown(SDL_SCANCODE_RALT)) {
        m_params.setApplyAndRecord_AltWheel(m_params.getAltWheel() + delta * SCROLL_SPEED, stateModifier);
    }
    else if (!bNoStandardScroll) {
        m_params.setApplyAndRecord_Wheel(m_params.getWheel() + delta * SCROLL_SPEED, stateModifier);
    }
}

void ConfigManager::onKeyPressed(SDL_Scancode scancode, char keysym, StateModifier& stateModifier) {
    // Text config
    if (m_currConfigType != ConfigType::TEXT || !m_textConfig.onKeyPressed(scancode, keysym, stateModifier)) {
        // Parameters
        if (!m_params.onKeyPressed(scancode, stateModifier)) {
            // Actions
            if (const Action* action = m_keyBindings.getAction(scancode)) {
                applyAndRecord_Action(*action, stateModifier);
            }
        }
    }
}

ConfigRef ConfigManager::getCurrentConfigRef() {
    switch (m_currConfigType) {
    case ConfigType::SHAPE_LAYOUT:
        return {
            m_shapeLayoutConfigs(m_currShapeIndex, m_currLayoutIndex).getName(),
            m_shapeLayoutConfigs(m_currShapeIndex, m_currLayoutIndex).getLayoutName(),
            ConfigType::SHAPE_LAYOUT
        };
        break;
    case ConfigType::SVG_LAYOUT:
        return {
            m_svgManager.getSVGName(m_currSvgIndex),
            m_svgManager.getLayoutName(m_currLayoutIndex),
            ConfigType::SVG_LAYOUT
        };
        break;
    case ConfigType::STANDALONE:
        return {
            m_standaloneConfigs[m_currStandaloneIndex].getName(),
            ConfigType::STANDALONE
        };
        break;
    case ConfigType::TEXT:
        return {
            m_textConfig.getName(),
            ConfigType::TEXT
        };
        break;
    }
}

void ConfigManager::applyAndRecord_Action(const Action& action, StateModifier& stateModifier) {
    switch (action.ref.type)
    {
    case ActionType::SHAPE:
        m_currConfigType = ConfigType::SHAPE_LAYOUT;
        m_currShapeIndex = action.index;
        m_bLastShapeWasSVG = false;
        stateModifier.recordChange({ StateChangeType::Action, action.ref });
        break;
    case ActionType::SVG_SHAPE:
        m_currConfigType = ConfigType::SVG_LAYOUT;
        m_currSvgIndex = action.index;
        m_bLastShapeWasSVG = true;
        stateModifier.recordChange({ StateChangeType::Action, action.ref });
        break;
    case ActionType::LAYOUT:
        m_currConfigType = m_bLastShapeWasSVG ? ConfigType::SVG_LAYOUT : ConfigType::SHAPE_LAYOUT;
        m_currLayoutIndex = action.index;
        stateModifier.recordChange({ StateChangeType::Action, action.ref });
        break;
    case ActionType::STANDALONE:
        m_currConfigType = ConfigType::STANDALONE;
        m_currStandaloneIndex = action.index;
        stateModifier.recordChange({ StateChangeType::Action, action.ref });
        break;
    case ActionType::TEXT:
        setCurrentConfigAsText();
        stateModifier.recordChange({ StateChangeType::Action, action.ref });
        break;
    case ActionType::REROLL_RANDOM:
        m_randParams.seed = 10.0f * MyRand::_m1to1();
        stateModifier.recordChange({ StateChangeType::Random_Seed, m_randParams.seed });
        break;
    default:
        break;
    }
    stateModifier.apply();
}

void ConfigManager::applyAndRecord_ActionRef(const ActionRef& actionRef, StateModifier& stateModifier) {
    if (const Action* action = m_keyBindings.getActionByRef(actionRef)) {
        applyAndRecord_Action(*action, stateModifier);
    }
    else {
        spdlog::warn("Couldn't find the action '{}' of type {}", actionRef.name, actionRef.type);
    }
}

void ConfigManager::applyAndRecord_ConfigRef(const ConfigRef& configRef, StateModifier& stateModifier) {
    switch (configRef.type) {
    case ConfigType::SHAPE_LAYOUT:
        applyAndRecord_ActionRef({ configRef.mainName,   ActionType::SHAPE }, stateModifier);
        applyAndRecord_ActionRef({ configRef.layoutName, ActionType::LAYOUT }, stateModifier);
        break;
    case ConfigType::SVG_LAYOUT:
        applyAndRecord_ActionRef({ configRef.mainName,   ActionType::SVG_SHAPE }, stateModifier);
        applyAndRecord_ActionRef({ configRef.layoutName, ActionType::LAYOUT }, stateModifier);
        break;
    case ConfigType::STANDALONE:
        applyAndRecord_ActionRef({ configRef.mainName, ActionType::STANDALONE }, stateModifier);
        break;
    case ConfigType::TEXT:
        setCurrentConfigAsText();
        break;
    }
}