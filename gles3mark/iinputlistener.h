/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include "input.h"

/**
 * \brief Input listener interface.
 *
 * inspired by libGDX
 */
class IInputListener {
public:
    virtual ~IInputListener() = default;

    // Called when a key was pressed - TODO rename to OnKeyPress
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