/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "app.h"

void App::OnInit() {
	gles3mark = std::unique_ptr<GLES3Mark>(new GLES3Mark()); // gles3mark = std::make_unique<GLES3Mark>();
	gles3mark->OnInit(state->window, state->activity->assetManager);
}

void App::OnResize(int w, int h) {

}

bool App::OnIdle() {
	if (gles3mark)
		return gles3mark->OnStep();
	else
		return true;
}

void App::OnQuit() {
	jmethodID finishMe = jniLink->GetMethodID("FinishMe", JNI_STRING_SIGNATURE);  // "(I)V" - int
	jstring jniText = jniLink->NewStringUTF(gles3mark->GetResultJSON());
	jniLink->CallVoidMethod(finishMe, jniText);
	jniLink->DeleteLocalRef(jniText);
}

void App::OnDestroy() {
	gles3mark.reset();
}

void App::OnTouchDown(int x, int y, int pointerId) {
	gles3mark->OnTouchDown(x, y, pointerId);
}

void App::OnTouchUp(int x, int y, int pointerId) {
	gles3mark->OnTouchUp(x, y, pointerId);
}

void App::OnTouchDragged(int x, int y, int dx, int dy, int pointerId) {
	gles3mark->OnTouchDragged(x, y, dx, dy, pointerId);
}
