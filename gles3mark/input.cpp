
#include "input.h"

void Input::RegisterKeyDown(KeyCode keyCode) {
    activeKeys[keyCode] = true;
}

void Input::RegisterKeyUp(KeyCode keyCode) {
    activeKeys[keyCode] = false;
}

bool Input::IsKeyDown(KeyCode keyCode) {
    return activeKeys[keyCode];
}

Input::KeyCode Input::OsKeyToKeyCode(int osKey) {
#ifdef _WIN32
    switch (osKey) {
        case VK_RETURN:     return KeyCode::Return;
        case VK_SPACE:      return KeyCode::Space;
        case VK_LSHIFT:     return KeyCode::LShift; // not working?
        case VK_TAB:        return KeyCode::Tab;
        case VK_LCONTROL:   return KeyCode::LCtrl;  // not working?
        case VK_ESCAPE:     return KeyCode::Escape;
        case VK_UP:         return KeyCode::Up;
        case VK_DOWN:       return KeyCode::Down;
        case VK_LEFT:       return KeyCode::Left;
        case VK_RIGHT:      return KeyCode::Right;
        case 'W':           return KeyCode::W;
        case 'S':           return KeyCode::S;
        case 'A':           return KeyCode::A;
        case 'D':           return KeyCode::D;
        case 'Q':           return KeyCode::Q;
    }
#endif
    return KeyCode::Default;
}