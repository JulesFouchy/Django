#include "ConfigParams.h"

#include "StateModifier.h"
#include "Recording/StateChange.h"

bool ConfigParams::onKeyPressed(SDL_Scancode scancode, StateModifier& stateModifier) {
    bool b = false;
    switch (scancode) {
    // Count
    case SDL_SCANCODE_KP_MINUS:
        count--;
        b = true;
        break;
    case SDL_SCANCODE_KP_PLUS:
        count++;
        b = true;
        break;
    case SDL_SCANCODE_KP_0:
    case SDL_SCANCODE_0:
        count = 0;
        b = true;
        break;
    case SDL_SCANCODE_KP_1:
    case SDL_SCANCODE_1:
        count = 1;
        b = true;
        break;
    case SDL_SCANCODE_KP_2:
    case SDL_SCANCODE_2:
        count = 2;
        b = true;
        break;
    case SDL_SCANCODE_KP_3:
    case SDL_SCANCODE_3:
        count = 4;
        b = true;
        break;
    case SDL_SCANCODE_KP_4:
    case SDL_SCANCODE_4:
        count = 8;
        b = true;
        break;
    case SDL_SCANCODE_KP_5:
    case SDL_SCANCODE_5:
        count = 16;
        b = true;
        break;
    case SDL_SCANCODE_KP_6: 
    case SDL_SCANCODE_6:
        count = 32;
        b = true;
        break;
    case SDL_SCANCODE_KP_7:
    case SDL_SCANCODE_7:
        count = 64;
        b = true;
        break;
    case SDL_SCANCODE_KP_8:
    case SDL_SCANCODE_8:
        count = 128;
        b = true;
        break;
    case SDL_SCANCODE_KP_9:
    case SDL_SCANCODE_9:
        count = 256;
        b = true;
        break;
    // LR
    case SDL_SCANCODE_LEFT:
        intLR--;
        b = true;
        break;
    case SDL_SCANCODE_RIGHT:
        intLR++;
        b = true;
        break;
    // UD
    case SDL_SCANCODE_DOWN:
        intUD--;
        b = true;
        break;
    case SDL_SCANCODE_UP:
        intUD++;
        b = true;
        break;
    }
    if (b) {
        stateModifier.apply();
        stateModifier.recordChange({ StateChangeType::Param_KeyPressed, scancode });
    }
    return b;
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