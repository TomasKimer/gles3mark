
#include "app.h"

void App::OnInit() /*override*/ {
	gles3mark = std::unique_ptr<GLES3Mark>(new GLES3Mark()); // gles3mark = std::make_unique<GLES3Mark>();
	gles3mark->OnInit(state->window, state->activity->assetManager);
}

void App::OnQuit() /*override*/ {
	jmethodID finishMe = jniLink->GetMethodID("FinishMe", JNI_STRING_SIGNATURE);  // "(I)V" - int
	jstring jniText = jniLink->NewStringUTF(gles3mark->GetResultXML());
	jniLink->CallVoidMethod(finishMe, jniText);
	jniLink->DeleteLocalRef(jniText);
}
