
#pragma once

class GLContext {

public:
    GLContext() {};
    virtual ~GLContext() {};
    
    virtual bool Create(void* osWnd) = 0;
    virtual void Destroy() = 0;
    virtual void Resize(int w, int h, bool vsync) = 0;
    virtual void Swap() = 0;

    int GetWidth() 		{ return mWidth;  }
    int GetHeight()		{ return mHeight; }

protected:
    int mWidth;
    int mHeight;
};
