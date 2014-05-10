/*
 * baseapp.h
 */

#ifdef ANDROID

#pragma once

#include <android_native_app_glue.h>
#include <vector>
#include <memory>

#include "jnilink.h"

/**
 * Shared state for the app.
 */
class BaseApp {
	struct Point {
		int x, y;  // float?
		Point():x(0),y(0){}
	}; //savedState;

	std::vector<Point>      touchDragPoints;
	bool					animating;
	bool					quit;

	// Process the next main command.
	static void handle_cmd(android_app* app, int32_t cmd);

	// Process the next main command.
	void HandleCommand(int32_t cmd);

	// Process the next input event.
	static int32_t handle_input(android_app* app, AInputEvent* event);

	// Process the next input event.
	int32_t HandleInput(AInputEvent* event);

protected:
	android_app*		     state;
	std::unique_ptr<JNILink> jniLink;

	virtual void OnInit() {}
	virtual void OnQuit() {}
	virtual void OnDestroy() {}
	virtual void OnResize(int w, int h) {}
	virtual bool OnIdle() { return true; }
	virtual void OnTouchDown(int x, int y, int pointerId) {}
	virtual void OnTouchUp(int x, int y, int pointerId) {}
	virtual void OnTouchDragged(int x, int y, int dx, int dy, int pointerId) {}

public:
	BaseApp(android_app* _state);
	virtual ~BaseApp() {}

	void Run();
};

#endif
