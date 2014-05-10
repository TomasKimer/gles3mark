
#pragma once

#include <string>

class IGLES3MarkLib {
public:
    virtual ~IGLES3MarkLib() = default;
    virtual bool OnInit(void* osWnd, void* ioContext = nullptr) = 0;
    virtual void OnResize(int w, int h) = 0;
    virtual bool OnStep() = 0;
    virtual std::string GetResultJSON() = 0;
};