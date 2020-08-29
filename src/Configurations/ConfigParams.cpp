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
    case SDL_SCANCODE_KP_PLUS:
        count++;
        b = true;
    case SDL_SCANCODE_KP_0:
    case SDL_SCANCODE_0:
        count = 0;
        b = true;
    case SDL_SCANCODE_KP_1:
    case SDL_SCANCODE_1:
        count = 1;
        b = true;
    case SDL_SCANCODE_KP_2:
    case SDL_SCANCODE_2:
        count = 2;
        b = true;
    case SDL_SCANCODE_KP_3:
    case SDL_SCANCODE_3:
        count = 4;
        b = true;
    case SDL_SCANCODE_KP_4:
    case SDL_SCANCODE_4:
        count = 8;
        b = true;
    case SDL_SCANCODE_KP_5:
    case SDL_SCANCODE_5:
        count = 16;
        b = true;
    case SDL_SCANCODE_KP_6: 
    case SDL_SCANCODE_6:
        count = 32;
        b = true;
    case SDL_SCANCODE_KP_7:
    case SDL_SCANCODE_7:
        count = 64;
        b = true;
    case SDL_SCANCODE_KP_8:
    case SDL_SCANCODE_8:
        count = 128;
        b = true;
    case SDL_SCANCODE_KP_9:
    case SDL_SCANCODE_9:
        count = 256;
        b = true;
    // LR
    case SDL_SCANCODE_LEFT:
        intLR = std::max(intLR-1, 1);
        b = true;
    case SDL_SCANCODE_RIGHT:
        intLR++;
        b = true;
    // UD
    case SDL_SCANCODE_DOWN:
        intUD--;
        return true;
    case SDL_SCANCODE_UP:
        intUD++;
        b = true;
    }
    if (b) {
        stateModifier.apply();
        stateModifier.recordChange({ StateChangeType::ConfigParams, scancode });
    }
    return b;
}