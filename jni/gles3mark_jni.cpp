// TODO rename to main and move BaseApp and GLContext + revise dir structure

#ifdef ANDROID

#include <jni.h>
#include <android_native_app_glue.h>

#include <sys/atomics.h>
static int _state; // for dalvik-to-native synchronization (UI thread to game thread)

#include "baseapp.h"


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
	Log::Msg("<<- MAIN START ->>");

	// Make sure glue isn't stripped by the linker.
	app_dummy();

	BaseApp* app = new BaseApp(state);

	app->Run();

    delete app;
}

// --- dalvik call to native test ---
/*
extern "C"
JNIEXPORT void JNICALL
Java_com_tomaskimer_gles3mark_BenchmarkActivity_nativeBlah(JNIEnv* env, jobject obj, jint data) {
	Log::Msg("<<- NATIVE BLAH ->>");

	// CAUTION: you're on the UI thread!
	__atomic_swap(data, &_state);
}

// can be called from any thread
// call every frame
int GetData() {
	return _state;
}
*/

// ---------------------------------------- OLD ------------------------------------
/*
#include <jni.h>
#include "gles3mark.h"

static GLES3Mark* gles3mark = NULL;

extern "C" {
    JNIEXPORT void JNICALL Java_com_tomaskimer_gles3mark_NativeLib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_tomaskimer_gles3mark_NativeLib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_tomaskimer_gles3mark_NativeLib_step(JNIEnv* env, jobject obj);
};

//extern "C"
JNIEXPORT void JNICALL
Java_com_tomaskimer_gles3mark_NativeLib_init(JNIEnv* env, jobject obj) {
    if (gles3mark) {
        delete gles3mark;
        gles3mark = NULL;
    }
    gles3mark = new GLES3Mark();
    gles3mark->OnInit();
}

JNIEXPORT void JNICALL
Java_com_tomaskimer_gles3mark_NativeLib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (gles3mark)
        gles3mark->OnResize(width, height);
}

JNIEXPORT void JNICALL
Java_com_tomaskimer_gles3mark_NativeLib_step(JNIEnv* env, jobject obj) {
    if (gles3mark)
        gles3mark->OnStep();
}
*/
#endif
