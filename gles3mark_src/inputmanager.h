

#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <map>

class InputManager {
public:
    enum class KeyCode {
        Unknown,
        Return,
        Space,
        LShift,
        Tab,
        LCtrl,
        Escape,
        Up,
        Down,
        Left,
        Right,
        W,
        S,
        A,
        D,
    };

    static KeyCode OsKeyToKeyCode(int osKey) {
#ifdef _WIN32
        switch (osKey) {
            case VK_RETURN:     return KeyCode::Return;
            case VK_SPACE:      return KeyCode::Space; 
            case VK_LSHIFT:     return KeyCode::LShift;
            case VK_TAB:        return KeyCode::Tab;   
            case VK_LCONTROL:   return KeyCode::LCtrl; 
            case VK_ESCAPE:     return KeyCode::Escape;
            case VK_UP:         return KeyCode::Up;    
            case VK_DOWN:       return KeyCode::Down;  
            case VK_LEFT:       return KeyCode::Left;  
            case VK_RIGHT:      return KeyCode::Right; 
            case 'W':           return KeyCode::W;     
            case 'S':           return KeyCode::S;     
            case 'A':           return KeyCode::A;     
            case 'D':           return KeyCode::D;     
        }
#endif
        return KeyCode::Unknown;
    }

    void RegisterKeyDown(KeyCode keyCode) { activeKeys[keyCode] = true ; }
    void RegisterKeyUp  (KeyCode keyCode) { activeKeys[keyCode] = false; }

    bool IsKeyDown(KeyCode keyCode) {
        return activeKeys[keyCode];
    }

private:
    std::map<KeyCode, bool> activeKeys;
};