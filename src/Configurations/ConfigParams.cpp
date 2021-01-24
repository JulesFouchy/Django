#include "ConfigParams.h"

#include "StateModifier.h"
#include "Recording/StateChange.h"
#include "Helper/Input.h"

bool ConfigParams::onKeyPressed(SDL_Scancode scancode, StateModifier& stateModifier) {
    bool b = false;
    switch (scancode) {
    // LR
    case SDL_SCANCODE_LEFT:
        LR--;
        b = true;
        break;
    case SDL_SCANCODE_RIGHT:
        LR++;
        b = true;
        break;
    // UD
    case SDL_SCANCODE_DOWN:
        UD--;
        b = true;
        break;
    case SDL_SCANCODE_UP:
        UD++;
        b = true;
        break;
    }
    if (b) {
        stateModifier.apply();
        stateModifier.recordChange({ StateChangeType::Param_KeyPressed, scancode });
    }
    return b;
}

void ConfigParams::onWheel(int delta, bool bNoStandardScroll, StateModifier& stateModifier) {
    if (Input::CtrlOrCmdIsDown()) {
        setApplyAndRecord_CtrlWheel(ctrlWheel + delta, stateModifier);
    }
    else if (Input::KeyIsDown(SDL_SCANCODE_LSHIFT) || Input::KeyIsDown(SDL_SCANCODE_RSHIFT)) {
        setApplyAndRecord_ShiftWheel(shiftWheel + delta, stateModifier);
    }
    else if (Input::KeyIsDown(SDL_SCANCODE_LALT) || Input::KeyIsDown(SDL_SCANCODE_RALT)) {
        setApplyAndRecord_AltWheel(altWheel + delta, stateModifier);
    }
    else if (!bNoStandardScroll) {
        setApplyAndRecord_Wheel(wheel + delta, stateModifier);
    }
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