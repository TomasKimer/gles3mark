
#pragma once

#include <memory>

#include "baseapp.h"
#include "../gles3mark/gles3mark.h"

class App : public BaseApp {
	std::unique_ptr<GLES3Mark> gles3mark;

	virtual void OnInit() override;
	virtual void OnResize(int w, int h) override {	}

	virtual bool OnIdle() override {
		//if (!glContext->HasDisplay()) return;
		return gles3mark->OnStep();
	}

	virtual void OnQuit() override;

	virtual void OnDestroy() override {
		gles3mark.reset();
	}

	virtual void OnTouchDown(int x, int y, int pointerId) override {
		gles3mark->OnTouchDown(x, y, pointerId);
	}

	virtual void OnTouchUp(int x, int y, int pointerId) override {
		gles3mark->OnTouchUp(x, y, pointerId);
	}

	virtual void OnTouchDragged(int x, int y, int dx, int dy, int pointerId) override {
		gles3mark->OnTouchDragged(x, y, dx, dy, pointerId);
	}


public:
	App(android_app* state): BaseApp(state) {}
	virtual ~App() {}
};
