

#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <map> // unordered_

class Input {
public:
    enum class KeyCode {
        Default,
        Escape, Return, Space, Tab,
        LShift, LCtrl,
        Up, Down, Left, Right,
        W, S, A, D,
        Q, E, R, T, Y, U, I, O, P,
    };

    enum class Button {
        Default,
        Left, Right, Middle,
        ScrollUp, ScrollDown,
    };

    static KeyCode OsKeyToKeyCode(int osKey) {
#ifdef _WIN32
        switch (osKey) {
            case VK_RETURN:     return KeyCode::Return;
            case VK_SPACE:      return KeyCode::Space;
            case VK_LSHIFT:     return KeyCode::LShift; // not working - why?
            case VK_TAB:        return KeyCode::Tab;
            case VK_LCONTROL:   return KeyCode::LCtrl;  // not working - why?
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

    void RegisterKeyDown(KeyCode keyCode) { activeKeys[keyCode] = true; }
    void RegisterKeyUp(KeyCode keyCode) { activeKeys[keyCode] = false; }

    bool IsKeyDown(KeyCode keyCode) {
        return activeKeys[keyCode];
    }

private:
    std::map<KeyCode, bool> activeKeys; // unordered_
};


class IInputListener {
public:
    virtual ~IInputListener() = default;

    // Called when a key was pressed
    virtual void OnKeyDown(Input::KeyCode keyCode) {}
    
    // Called when a key was released
    virtual void OnKeyUp(Input::KeyCode keyCode) {}

    // Called when a key was typed
    virtual void OnKeyTyped(char character) {}
    
    // Called when the screen was touched or a mouse button was pressed.
    virtual void OnTouchDown(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) {}
    
    // Called when a finger was lifted or a mouse button was released.
    virtual void OnTouchUp(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) {}
    
    // Called when a finger or the mouse was dragged.
    virtual void OnTouchDragged(int screenX, int screenY, int dx, int dy, int pointer = -1) {}
    
    // Called when the mouse was moved without any buttons being pressed.
    virtual void OnMouseMoved(int screenX, int screenY) {}

    // Called when the mouse wheel was scrolled.
    virtual void OnScrolled(int amount) {}
};
