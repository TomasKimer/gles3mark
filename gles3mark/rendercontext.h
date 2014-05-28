/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

/**
 * \brief Abstract render context.
 */
class RenderContext {
public:
    RenderContext(): mWidth(0), mHeight(0),
                     minSwapInterval(-1), maxSwapInterval(-1),
                     versionMajor(0), versionMinor(0) {}
    virtual ~RenderContext() = default;
    
    virtual bool Create(void* osWnd) = 0;
    virtual void Destroy() = 0;
    virtual void Resize(int w, int h) = 0;
    virtual void Swap() = 0;
    virtual void SetVsync(bool enabled) = 0;

    int GetWidth()  const { return mWidth;  }
    int GetHeight() const { return mHeight; }

    int GetSwapIntervalMin() const { return minSwapInterval; }
    int GetSwapIntervalMax() const { return maxSwapInterval; }

    int GetVersionMajor() const { return versionMajor; }
    int GetVersionMinor() const { return versionMinor; }

protected:
    int mWidth, mHeight;
    int minSwapInterval, maxSwapInterval;
    int versionMajor, versionMinor;
};
