/*
 * baseapp.h
 */

#ifdef ANDROID

#pragma once

#include <android_native_app_glue.h>
#include <memory>
#include <vector>
#include "../gles3mark_src/gles3mark.h"

/**
 * Shared state for our app.
 */
class BaseApp {
	android_app*			   state;
	std::unique_ptr<GLES3Mark> gles3mark;

	JNIEnv 			*env; // game thread env (vs main thread env: state->activity->env)
	jobject			thiz;
	jclass			clazz;

	bool			animating;
	bool			quit;

	/**
	 * Our saved state data.
	 */
	struct Point {
		int x, y;
		Point():x(0),y(0){}
	} savedState;


	/**
	 * Process the next main command.
	 */
	static void handle_cmd(android_app* app, int32_t cmd);

	/**
	 * Process the next main command.
	 */
	void HandleCommand(int32_t cmd);

	/**
	 * Process the next input event.
	 */
	static int32_t handle_input(android_app* app, AInputEvent* event);

	/**
	 * Process the next input event.
	 */
	int32_t HandleInput(AInputEvent* event);

protected:
	//int score;
	std::vector<Point> touchDragPoints;

	virtual void OnStartup() {}
	virtual void OnQuit()   {}
	virtual void OnResize(int w, int h) {}
	virtual bool OnIdle() {
		//if (!glContext->HasDisplay()) return;
		return gles3mark->OnStep();
	}

	// JNI is running the equivalent of the following Java code: activity.showToastAlert(text);
	bool showToast(const char* text);

public:
	BaseApp(android_app* _state);
	virtual ~BaseApp() {}

	void Run();
};

#endif

