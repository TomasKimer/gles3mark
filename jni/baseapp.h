/*
 * baseapp.h
 */

#ifdef ANDROID

#pragma once

#include <jni.h>
#include <android_native_app_glue.h>
#include "../gles3mark_src/log.h"
#include "../gles3mark_src/gles3mark.h"

/**
 * Shared state for our app.
 */
class BaseApp {
	android_app* state;
	GLES3Mark* gles3mark;
	//GLES3ContextEGL* glContext;

	/**
	 * Our saved state data.
	 */
	struct SavedState {
		int x, y;
		SavedState():x(0),y(0){}
	} savedState;
	bool animating;

	/**
	 * Process the next main command.
	 */
	static void handle_cmd(android_app* app, int32_t cmd) {
		BaseApp* me = (BaseApp*)app->userData;
		me->HandleCommand(cmd);
	}

	/**
	 * Process the next main command.
	 */
	void HandleCommand(int32_t cmd) {
		switch (cmd) {

		// The system has asked us to save our current state.  Do so.
		case APP_CMD_SAVE_STATE:
			state->savedState = malloc(sizeof(SavedState));
			*((SavedState*)state->savedState) = savedState;
			state->savedStateSize = sizeof(SavedState);
			Log::Stream() << "State saved: x: " << savedState.x << ", y: " << savedState.y;
			break;

		// The window is being shown, get it ready.
		case APP_CMD_INIT_WINDOW:
			if (state->window != NULL) {
				Log::Msg("<<- CMD INIT WINDOW ->>");
				if (!gles3mark)
					gles3mark = new GLES3Mark();
				gles3mark->OnInit(state->window, state->activity->assetManager);
				gles3mark->OnResize(gles3mark->GetGLContext()->GetWidth(), gles3mark->GetGLContext()->GetHeight());
				//glContext->Create(state->window);
				OnIdle();
			}
			break;

		// The window is being hidden or closed, clean it up.
		case APP_CMD_TERM_WINDOW:
			Log::Msg("<<- CMD TERM WINDOW ->>");
			//glContext->Destroy();
			if (gles3mark) {
				 delete gles3mark;
				 gles3mark = NULL;
			}
			animating = false;
			break;

		// When our app gains focus, we start monitoring the accelerometer.
		case APP_CMD_GAINED_FOCUS:
			Log::Msg("<<- CMD GAINED FOCUS  ->>");
			break;

		// When our app loses focus, we stop monitoring the accelerometer. This is to avoid consuming battery while not being used.
		case APP_CMD_LOST_FOCUS:
			Log::Msg("<<- CMD LOST FOCUS ->>");
			animating = false; // Also stop animating.
			OnIdle();
			break;
		}
	}

	/**
	 * Process the next input event.
	 */
	static int32_t handle_input(android_app* app, AInputEvent* event) {
		BaseApp* me = (BaseApp*)app->userData;
		return me->HandleInput(event);
	}

	/**
	 * Process the next input event.
	 */
	int32_t HandleInput(AInputEvent* event) {
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
			animating = !animating;
			savedState.x = AMotionEvent_getX(event, 0);
			savedState.y = AMotionEvent_getY(event, 0);
			return 1;
		}
		return 0;
	}

protected:

	virtual void OnCreate() {}
	virtual void OnQuit()   {}

	/**
	 * Just the current frame in the display.
	 */
	virtual void OnIdle() {
		//if (!glContext->HasDisplay())
		//	return;

		if (gles3mark)
			gles3mark->OnStep();

		//glClearColor((float)savedState.x/glContext->GetWidth(), 0.f, (float)savedState.y/glContext->GetHeight(), 1.f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glContext->Swap();
	}

public:
	BaseApp(android_app* _state): state(_state), /*glContext(NULL),*/ animating(false), gles3mark(NULL) {
		state->userData = this;
		state->onAppCmd = handle_cmd;
		state->onInputEvent = handle_input;

		// We are starting with a previous saved state; restore from it.
		if (state->savedState != NULL) {
			savedState = *(SavedState*)state->savedState;
			Log::Stream() << "State loaded: x: " << savedState.x << ", y: " << savedState.y;
		}

		//glContext = new GLES3ContextEGL();
		if (!gles3mark) {
			gles3mark = new GLES3Mark();
		}

		//state->activity->internalDataPath;
	}

	virtual ~BaseApp() {}

	void Run() {
	    while (true) {
	        // Read all pending events.
	        int ident, events;
	        android_poll_source* source;

	        // If not animating, we will block forever waiting for events.
	        // If animating, we loop until all events are read, then continue to draw the next frame of animation.
	        while ((ident = ALooper_pollAll(animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {

	            // Process this event.
	            if (source != NULL) {
	                source->process(state, source);
	            }

	            // If a sensor has data, process it now.
	            if (ident == LOOPER_ID_USER) {
	            }

	            // Check if we are exiting.
	            if (state->destroyRequested != 0) {
	            	Log::Msg("<<- DESTROY REQUESTED ->>");
	            	//glContext->Destroy();
	            	if (gles3mark) {
	            		delete gles3mark;
	            		gles3mark = NULL;
	            	}
	            	animating = false;
	    			showToast("Exitting");
	                return;
	            }
	        }

	        if (animating) {
	            // Done with events; draw next animation frame.
	            // Update game state

	            // Drawing is throttled to the screen update rate, so there is no need to do timing here.
	        	OnIdle();
	        }
	    }
	}

	bool showToast(const char* text) {
		// JNI is running the equivalent of the following Java code: activity.showToastAlert(text);

		// only once
		JNIEnv *env; // = state->activity->env;
		if (state->activity->vm->AttachCurrentThread(&env, NULL) != 0)
			Log::Msg("AttachCurrentThread");
		jobject thiz = state->activity->clazz;
		jclass clazz = env->GetObjectClass(thiz);

		// specific
		jmethodID showToastAlert = env->GetMethodID(clazz, "showToastAlert", "(Ljava/lang/String;)V");
		jstring jniText = env->NewStringUTF(text);
		env->CallVoidMethod(thiz, showToastAlert, jniText);
		env->DeleteLocalRef(jniText);


		// only once
		// detach (when thread exists) - else error: "native thread exited without detaching"
		state->activity->vm->DetachCurrentThread();

		// Check nvidia's own native_app_glue implementation for encapsulation
		// and check nvidia's lifecycle !! can be glContext moved to the general engine?
/*		jstring jniText = mApp->appThreadEnv->NewStringUTF(text);   				        EXCEPTION_RETURN(mApp->appThreadEnv);
	    jclass thisClass = mApp->appThreadEnv->GetObjectClass(mApp->appThreadThis);         EXCEPTION_RETURN(mApp->appThreadEnv);
		jmethodID showToastAlert = mApp->appThreadEnv->GetMethodID(thisClass, "showToastAlert", "(Ljava/lang/String;)V"); EXCEPTION_RETURN(mApp->appThreadEnv);
		mApp->appThreadEnv->CallVoidMethod(mApp->appThreadThis, showToastAlert, jniText); 	EXCEPTION_RETURN(mApp->appThreadEnv);
*/
		return true;
	}
};

#endif

