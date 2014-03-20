/*
 * baseapp.cpp
 */

#include "baseapp.h"
#include "../gles3mark_src/log.h"


BaseApp::BaseApp(android_app* _state)
	: state(_state), animating(false), quit(false) {

	Log::Msg("<<- MAIN START ->>");

	app_dummy(); // Make sure glue isn't stripped by the linker.

	state->userData = this;
	state->onAppCmd = handle_cmd;
	state->onInputEvent = handle_input;

	// We are starting with a previous saved state; restore from it.
	if (state->savedState != nullptr) {
		savedState = *(SavedState*)state->savedState;
		Log::Stream() << "State loaded: x: " << savedState.x << ", y: " << savedState.y;
	}

	if (state->activity->vm->AttachCurrentThread(&env, nullptr) == 0) {
		thiz = state->activity->clazz;
		clazz = env->GetObjectClass(thiz);
	} else {
		Log::Msg("AttachCurrentThread Failed");
	}

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
			if (source != nullptr) {
				source->process(state, source);
			}

			// If a sensor has data, process it now.
			if (ident == LOOPER_ID_USER) {
			}

			// Check if we are exiting.  Which is the case once we called ANativeActivity_finish
			if (state->destroyRequested != 0) {
				Log::Msg("LOOPER DESTROY REQUESTED"); // Quit our app stuff here

				//animating = false;
				//showToast("Exitting");

				// detach from current thread (when thread exists) - else error: "native thread exited without detaching"
				state->activity->vm->DetachCurrentThread();

				return; // return the main, so we get back to our java activity which called the nativeactivity
			}
		}

		// Done with events; draw next animation frame.
		if (animating) {
			// Update game state

			// Drawing is throttled to the screen update rate, so there is no need to do timing here.
			OnIdle();
		}

		// if our app told us to finish
		if (quit) {
			ANativeActivity_finish(state->activity);
		}
	}
}

/**
 * Process the next input event.
 */
int32_t BaseApp::HandleInput(AInputEvent* event) {
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		animating = !animating;
		savedState.x = AMotionEvent_getX(event, 0);
		savedState.y = AMotionEvent_getY(event, 0);

		Exit(934);

		return 1;
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
			gles3mark = std::unique_ptr<GLES3Mark>(new GLES3Mark()); // gles3mark = std::make_unique<GLES3Mark>();
			gles3mark->OnInit(state->window, state->activity->assetManager);
			gles3mark->OnResize(gles3mark->GetGLContext()->GetWidth(), gles3mark->GetGLContext()->GetHeight());
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
		gles3mark.reset();
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
		state->savedState = malloc(sizeof(SavedState));
		*((SavedState*)state->savedState) = savedState;
		state->savedStateSize = sizeof(SavedState);
		//Log::Stream() << "State saved: x: " << savedState.x << ", y: " << savedState.y;
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
	 * Command from main thread: the current ANativeWindow has been resized.
	 * Please redraw with its new size.
	 */
	case APP_CMD_WINDOW_RESIZED:
		Log::Msg("APP_CMD_WINDOW_RESIZED");
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
		break;

	/**
	 * Command from main thread: the app's activity has been paused.
	 */
	case APP_CMD_PAUSE:
		Log::Msg("APP_CMD_PAUSE");
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

// JNI
void BaseApp::Exit(int score) {
	jmethodID FinishMe = env->GetMethodID(clazz, "FinishMe", "(I)V");
	env->CallVoidMethod(thiz, FinishMe, score);
	quit = true;
}

// JNI is running the equivalent of the following Java code: activity.showToastAlert(text);
bool BaseApp::showToast(const char* text) {
	jmethodID showToastAlert = env->GetMethodID(clazz, "showToastAlert", "(Ljava/lang/String;)V");
	jstring jniText = env->NewStringUTF(text);
	env->CallVoidMethod(thiz, showToastAlert, jniText);
	env->DeleteLocalRef(jniText);

	// Check nvidia's own native_app_glue implementation for encapsulation
/*		jstring jniText = mApp->appThreadEnv->NewStringUTF(text);   				        EXCEPTION_RETURN(mApp->appThreadEnv);
    jclass thisClass = mApp->appThreadEnv->GetObjectClass(mApp->appThreadThis);         EXCEPTION_RETURN(mApp->appThreadEnv);
	jmethodID showToastAlert = mApp->appThreadEnv->GetMethodID(thisClass, "showToastAlert", "(Ljava/lang/String;)V"); EXCEPTION_RETURN(mApp->appThreadEnv);
	mApp->appThreadEnv->CallVoidMethod(mApp->appThreadThis, showToastAlert, jniText); 	EXCEPTION_RETURN(mApp->appThreadEnv);
*/
	return true;
}

