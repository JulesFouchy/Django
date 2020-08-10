#include "ConfigParams.h"

bool ConfigParams::onKeyPressed(SDL_Scancode scancode) {
    switch (scancode) {
    // Count
    case SDL_SCANCODE_KP_MINUS:
        count--;
        return true;
    case SDL_SCANCODE_KP_PLUS:
        count++;
        return true;
    case SDL_SCANCODE_KP_0:
    case SDL_SCANCODE_0:
        count = 0;
        return true;
    case SDL_SCANCODE_KP_1:
    case SDL_SCANCODE_1:
        count = 1;
        return true;
    case SDL_SCANCODE_KP_2:
    case SDL_SCANCODE_2:
        count = 2;
        return true;
    case SDL_SCANCODE_KP_3:
    case SDL_SCANCODE_3:
        count = 4;
        return true;
    case SDL_SCANCODE_KP_4:
    case SDL_SCANCODE_4:
        count = 8;
        return true;
    case SDL_SCANCODE_KP_5:
    case SDL_SCANCODE_5:
        count = 16;
        return true;
    case SDL_SCANCODE_KP_6: 
    case SDL_SCANCODE_6:
        count = 32;
        return true;
    case SDL_SCANCODE_KP_7:
    case SDL_SCANCODE_7:
        count = 64;
        return true;
    case SDL_SCANCODE_KP_8:
    case SDL_SCANCODE_8:
        count = 128;
        return true;
    case SDL_SCANCODE_KP_9:
    case SDL_SCANCODE_9:
        count = 256;
        return true;
    // LR
    case SDL_SCANCODE_LEFT:
        intLR = std::max(intLR-1, 1);
        return true;
    case SDL_SCANCODE_RIGHT:
        intLR++;
        return true;
    // UD
    case SDL_SCANCODE_DOWN:
        intUD--;
        return true;
    case SDL_SCANCODE_UP:
        intUD++;
        return true;
    default:
        return false;
    }
}