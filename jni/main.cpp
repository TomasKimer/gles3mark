
#ifdef ANDROID

#include <android_native_app_glue.h>
#include <memory>
#include "app.h"

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue. It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
	std::unique_ptr<App> app(new App(state));

	app->Run();
}

#endif
