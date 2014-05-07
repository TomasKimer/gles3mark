/*
 * baseapp.cpp
 */

#include "baseapp.h"
#include "../gles3mark/log.h"


BaseApp::BaseApp(android_app* _state)
	: state(_state), animating(false), quit(false),
	  touchDragPoints(10, Point()) {

	Log::Msg("<<- MAIN START ->>");

	app_dummy(); // Make sure glue isn't stripped by the linker.

	state->userData = this;
	state->onAppCmd = handle_cmd;
	state->onInputEvent = handle_input;

	// We are starting with a previous saved state; restore from it.
	if (state->savedState != nullptr) {
		savedState = *(Point*)state->savedState;
		Log::Stream() << "State loaded: x: " << savedState.x << ", y: " << savedState.y;
	}

	jniLink = std::unique_ptr<JNILink>(new JNILink(state));

	//state->activity->internalDataPath;
}

void BaseApp::Run() {
	// our main loop for the app. Will only return once the game is really finished.
	while (true) {
		// Read all pending events.
		int ident, events;
		android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue to draw the next frame of animation.
		while ((ident = ALooper_pollAll(animating ? 0 : -1, nullptr, &events, (void**)&source)) >= 0) {

			// Process this event.
			if (source) {
				source->process(state, source);
			}

			// If a sensor has data, process it now.
			if (ident == LOOPER_ID_USER) {
				Log::Msg("LOOPER_ID_USER");
			}

			// Check if we are exiting.  Which is the case once we called ANativeActivity_finish
			if (state->destroyRequested != 0) {
				Log::Msg("LOOPER DESTROY REQUESTED"); // Quit our app stuff here

				//animating = false;
				//showToast("Exiting");

				// detach from current thread (when thread exists) - else error: "native thread exited without detaching"
				jniLink.reset();

				return; // return the main, so we get back to our java activity which called the nativeactivity
			}
		}

		// Done with events; draw next animation frame.
		if (animating && !quit) {
			// Update game state

			// Drawing is throttled to the screen update rate, so there is no need to do timing here.
			if (!OnIdle()) {
				quit = true;
				OnQuit();
			}
		}

		// if our app told us to finish
		if (quit) {
			//ANativeActivity_finish(state->activity);   // Failed writing to work fd: Try again
		}
	}
}

/**
 * Process the next input event.
 */
// TODO float !!!
int32_t BaseApp::HandleInput(AInputEvent* event) {
	if (quit)
		return 1;

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

		int action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
		int pointerIndex, pointerId, x, y;

		if (action != AMOTION_EVENT_ACTION_MOVE) {
			pointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			pointerId = AMotionEvent_getPointerId(event, pointerIndex);

			x = AMotionEvent_getX(event, pointerIndex);
			y = AMotionEvent_getY(event, pointerIndex);
		}

		switch (action) {
		case AMOTION_EVENT_ACTION_DOWN:
		case AMOTION_EVENT_ACTION_POINTER_DOWN:

			OnTouchDown(x, y, pointerId);  // bool? if false return 0?

			touchDragPoints[pointerId].x = x;
			touchDragPoints[pointerId].y = y;

			break;

		case AMOTION_EVENT_ACTION_UP:
		case AMOTION_EVENT_ACTION_POINTER_UP:
		case AMOTION_EVENT_ACTION_CANCEL:

			OnTouchUp(x, y, pointerId);

			break;

		case AMOTION_EVENT_ACTION_MOVE:
			for (int i = 0; i < AMotionEvent_getPointerCount(event); ++i) {
				pointerIndex = i;
				pointerId = AMotionEvent_getPointerId(event, pointerIndex);

				x = AMotionEvent_getX(event, pointerIndex);
				y = AMotionEvent_getY(event, pointerIndex);

				int dx = x - touchDragPoints[pointerId].x;
				int dy = y - touchDragPoints[pointerId].y;

				OnTouchDragged(x, y, dx, dy, pointerId);

				touchDragPoints[pointerId].x = x;
				touchDragPoints[pointerId].y = y;
			}
			break;
		}

		return 1;  // event handled
	}

	return 0;
}

/**
 * Process the next main command.
 */
void BaseApp::HandleCommand(int32_t cmd) {

	switch (cmd) {

	/**
	 * Command from main thread: a new ANativeWindow is ready for use.  Upon
	 * receiving this command, android_app->window will contain the new window
	 * surface.
	 */
	case APP_CMD_INIT_WINDOW: // The window is being shown, get it ready.
		Log::Msg("APP_CMD_INIT_WINDOW");
		if (state->window) {
			//if (!gles3mark)
			//	gles3mark = new GLES3Mark();
			OnInit();

    		//gles3mark->OnResize(gles3mark->GetContext()->GetWidth(), gles3mark->GetContext()->GetHeight());
			//glContext->Create(state->window);
			animating = true;

			//OnIdle();
		}
		break;

	/**
	* Command from main thread: the existing ANativeWindow needs to be
	* terminated.  Upon receiving this command, android_app->window still
	* contains the existing window; after calling android_app_exec_cmd
	* it will be set to NULL.
	*/
	case APP_CMD_TERM_WINDOW:   //The window is being hidden or closed, clean it up.
		Log::Msg("APP_CMD_TERM_WINDOW");
		//glContext->Destroy();
		OnDestroy();
		animating = false;
		break;

	/**
	 * Command from main thread: the app's activity window has gained
	 * input focus.
	 */
	case APP_CMD_GAINED_FOCUS:  // When our app gains focus, we start monitoring the accelerometer.
		Log::Msg("APP_CMD_GAINED_FOCUS");
		animating = true;
		break;

	/**
	 * Command from main thread: the app's activity window has lost
	 * input focus.
	 */
	case APP_CMD_LOST_FOCUS: // When our app loses focus, we stop monitoring the accelerometer. This is to avoid consuming battery while not being used.
		Log::Msg("APP_CMD_LOST_FOCUS");
		animating = false; // Also stop animating.
		//OnIdle();
		break;

	/**
	 * Command from main thread: the app should generate a new saved state
	 * for itself, to restore from later if needed.  If you have saved state,
	 * allocate it with malloc and place it in android_app.savedState with
	 * the size in android_app.savedStateSize.  The will be freed for you
	 * later.
	 */
	case APP_CMD_SAVE_STATE:
		Log::Msg("APP_CMD_SAVE_STATE");
		state->savedState = malloc(sizeof(Point));
		*((Point*)state->savedState) = savedState;
		state->savedStateSize = sizeof(Point);
		//Log::Stream() << "State saved: x: " << savedState.x << ", y: " << savedState.y;
		break;

	/**
	 * Command from main thread: the current ANativeWindow has been resized.
	 * Please redraw with its new size.
	 */
	case APP_CMD_WINDOW_RESIZED:
		Log::Msg("APP_CMD_WINDOW_RESIZED");
		OnResize(-1, -1);
		break;

	/**
	 * Command from main thread: the system needs that the current ANativeWindow
	 * be redrawn.  You should redraw the window before handing this to
	 * android_app_exec_cmd() in order to avoid transient drawing glitches.
	 */
	case APP_CMD_WINDOW_REDRAW_NEEDED:
		Log::Msg("APP_CMD_WINDOW_REDRAW_NEEDED");
		break;

	/**
	 * Command from main thread: the AInputQueue has changed.  Upon processing
	 * this command, android_app->inputQueue will be updated to the new queue
	 * (or NULL).
	 */
	case APP_CMD_INPUT_CHANGED:
		Log::Msg("APP_CMD_INPUT_CHANGED");
		break;

	/**
	 * Command from main thread: the content area of the window has changed,
	 * such as from the soft input window being shown or hidden.  You can
	 * find the new content rect in android_app::contentRect.
	 */
	case APP_CMD_CONTENT_RECT_CHANGED:
		Log::Msg("APP_CMD_CONTENT_RECT_CHANGED");
		break;

	/**
	 * Command from main thread: the current device configuration has changed.
	 */
	case APP_CMD_CONFIG_CHANGED:
		Log::Msg("APP_CMD_CONFIG_CHANGED");
		break;

	/**
	 * Command from main thread: the system is running low on memory.
	 * Try to reduce your memory use.
	 */
	case APP_CMD_LOW_MEMORY:
		Log::Msg("APP_CMD_LOW_MEMORY");
		break;

	/**
	 * Command from main thread: the app's activity has been started.
	 */
	case APP_CMD_START:
		Log::Msg("APP_CMD_START");
		break;

	/**
	 * Command from main thread: the app's activity has been resumed.
	 */
	case APP_CMD_RESUME:
		Log::Msg("APP_CMD_RESUME");
		animating = true;
		break;

	/**
	 * Command from main thread: the app's activity has been paused.
	 */
	case APP_CMD_PAUSE:
		Log::Msg("APP_CMD_PAUSE");
		animating = false;
		break;

	/**
	 * Command from main thread: the app's activity has been stopped.
	 */
	case APP_CMD_STOP:
		Log::Msg("APP_CMD_STOP");
		break;

	/**
	 * Command from main thread: the app's activity is being destroyed,
	 * and waiting for the app thread to clean up and exit before proceeding.
	 */
	case APP_CMD_DESTROY:
		Log::Msg("APP_CMD_DESTROY");
		break;
	}
}

/**
 * Process the next main command.
 */
void BaseApp::handle_cmd(android_app* app, int32_t cmd) {
	BaseApp* me = (BaseApp*)app->userData;
	me->HandleCommand(cmd);
}

/**
 * Process the next input event.
 */
int32_t BaseApp::handle_input(android_app* app, AInputEvent* event) {
	BaseApp* me = (BaseApp*)app->userData;
	return me->HandleInput(event);
}

// JNI is running the equivalent of the following Java code: activity.showToastAlert(text);
bool BaseApp::showToast(const std::string& text) {
	jmethodID showToastAlert = jniLink->GetMethodID("showToastAlert", JNI_STRING_SIGNATURE);
	jstring jniText = jniLink->NewStringUTF(text);
	jniLink->CallVoidMethod(showToastAlert, jniText);
	jniLink->DeleteLocalRef(jniText);

	// Check nvidia's own native_app_glue implementation for encapsulation
/*		jstring jniText = mApp->appThreadEnv->NewStringUTF(text);   				        EXCEPTION_RETURN(mApp->appThreadEnv);
    jclass thisClass = mApp->appThreadEnv->GetObjectClass(mApp->appThreadThis);         EXCEPTION_RETURN(mApp->appThreadEnv);
	jmethodID showToastAlert = mApp->appThreadEnv->GetMethodID(thisClass, "showToastAlert", "(Ljava/lang/String;)V"); EXCEPTION_RETURN(mApp->appThreadEnv);
	mApp->appThreadEnv->CallVoidMethod(mApp->appThreadThis, showToastAlert, jniText); 	EXCEPTION_RETURN(mApp->appThreadEnv);
*/
	return true;
}
