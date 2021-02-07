#include "ConfigParams.h"

#include "StateModifier.h"
#include "Recording/StateChange.h"
#include <Cool/App/Input.h>

bool ConfigParams::onKeyPressed(int keycode, int mods, StateModifier& stateModifier) {
    bool b = false;
    switch (keycode) {
    // LR
    case GLFW_KEY_LEFT:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            LR -= 6;
        else
            LR--;
        b = true;
        break;
    case GLFW_KEY_RIGHT:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            LR += 6;
        else
            LR++;
        b = true;
        break;
    // UD
    case GLFW_KEY_DOWN:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            UD -= 6;
        else
            UD--;
        b = true;
        break;
    case GLFW_KEY_UP:
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT))
            UD += 6;
        else
            UD++;
        b = true;
        break;
    }
    if (b) {
        //stateModifier.apply();
        //stateModifier.recordChange({ StateChangeType::Param_KeyPressed, scancode }); // TODO FIXME : record new value instead of key press
    }
    return b;
}

void ConfigParams::onWheel(int delta, bool bNoStandardScroll, StateModifier& stateModifier) {
    //if (Input::CtrlOrCmdIsDown()) {
    //    setApplyAndRecord_CtrlWheel(ctrlWheel + delta, stateModifier);
    //}
    //else if (Input::ShiftIsDown()) {
    //    setApplyAndRecord_ShiftWheel(shiftWheel + delta, stateModifier);
    //}
    //else if (Input::KeyIsDown(SDL_SCANCODE_LALT) || Input::KeyIsDown(SDL_SCANCODE_RALT)) {
    //    setApplyAndRecord_AltWheel(altWheel + delta, stateModifier);
    //}
    //else if (!bNoStandardScroll) {
    //    setApplyAndRecord_Wheel(wheel + delta, stateModifier);
    //}
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