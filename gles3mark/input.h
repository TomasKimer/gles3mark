/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <map> // unordered_map -> problem with 'enum class' key

/**
 * \brief Windows input for debugging.
 */
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

    void RegisterKeyDown(KeyCode keyCode);
    void RegisterKeyUp  (KeyCode keyCode);

    bool IsKeyDown      (KeyCode keyCode);

    static KeyCode OsKeyToKeyCode(int osKey);

private:
    std::map<KeyCode, bool> activeKeys;
};
