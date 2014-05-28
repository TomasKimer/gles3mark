/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <memory>

#include "baseapp.h"
#include "../gles3mark/gles3mark.h"

/**
 * \brief Benchmark native app.
 */
class App : public BaseApp {
	std::unique_ptr<GLES3Mark> gles3mark;

	virtual void OnInit() override;
	virtual void OnResize(int w, int h) override;
	virtual bool OnIdle() override;
	virtual void OnQuit() override;
	virtual void OnDestroy() override;
	virtual void OnTouchDown(int x, int y, int pointerId) override;
	virtual void OnTouchUp(int x, int y, int pointerId) override;
	virtual void OnTouchDragged(int x, int y, int dx, int dy, int pointerId) override;

public:
	App(android_app* state): BaseApp(state) {}
	virtual ~App() {}
};
