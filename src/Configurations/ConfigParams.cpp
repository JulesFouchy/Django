#include "ConfigParams.h"

#include "StateModifier.h"
#include "Recording/StateChange.h"
#include <Cool/App/Input.h>

using namespace Cool;

bool ConfigParams::onKeyPressed(int keycode, int mods, StateModifier& stateModifier) {
    switch (keycode) {
    // LR
    case GLFW_KEY_LEFT:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            setApplyAndRecord_LR(LR - 6, stateModifier);
        else
            setApplyAndRecord_LR(LR - 1, stateModifier);
        return true;
    case GLFW_KEY_RIGHT:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            setApplyAndRecord_LR(LR + 6, stateModifier);
        else
            setApplyAndRecord_LR(LR + 1, stateModifier);
        return true;
    // UD
    case GLFW_KEY_DOWN:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            setApplyAndRecord_UD(UD - 6, stateModifier);
        else
            setApplyAndRecord_UD(UD - 1, stateModifier);
        return true;
    case GLFW_KEY_UP:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            setApplyAndRecord_UD(UD + 6, stateModifier);
        else
            setApplyAndRecord_UD(UD + 1, stateModifier);
        return true;
     //
    default:
        return false;
    }
}

void ConfigParams::onWheel(int delta, bool bNoStandardScroll, StateModifier& stateModifier) {
    if (Input::CtrlOrCmdIsDown()) {
        setApplyAndRecord_CtrlWheel(ctrlWheel + delta, stateModifier);
    }
    else if (Input::ShiftIsDown()) {
        setApplyAndRecord_ShiftWheel(shiftWheel + delta, stateModifier);
    }
    else if (Input::AltIsDown()) {
        setApplyAndRecord_AltWheel(altWheel + delta, stateModifier);
    }
    else if (!bNoStandardScroll) {
        setApplyAndRecord_Wheel(wheel + delta, stateModifier);
    }
}

void ConfigParams::setApplyAndRecord_LR(int value, StateModifier& stateModifier) {
    LR = value;
    stateModifier.apply();
    stateModifier.recordChange({ StateChangeType::Param_LR, value });
}

void ConfigParams::setApplyAndRecord_UD(int value, StateModifier& stateModifier) {
    UD = value;
    stateModifier.apply();
    stateModifier.recordChange({ StateChangeType::Param_UD, value });
}

void ConfigParams::setApplyAndRecord_Wheel(int value, StateModifier& stateModifier) {
    wheel = value;
    stateModifier.apply();
    stateModifier.recordChange({StateChangeType::Param_Wheel, value });
}

void ConfigParams::setApplyAndRecord_CtrlWheel(int value, StateModifier& stateModifier) {
    ctrlWheel = value;
    stateModifier.apply();
    stateModifier.recordChange({ StateChangeType::Param_CtrlWheel, value });
}

void ConfigParams::setApplyAndRecord_ShiftWheel(int value, StateModifier& stateModifier) {
    shiftWheel = value;
    stateModifier.apply();
    stateModifier.recordChange({ StateChangeType::Param_ShiftWheel, value });
}

void ConfigParams::setApplyAndRecord_AltWheel(int value, StateModifier& stateModifier) {
    altWheel = value;
    stateModifier.apply();
    stateModifier.recordChange({ StateChangeType::Param_AltWheel, value });
}