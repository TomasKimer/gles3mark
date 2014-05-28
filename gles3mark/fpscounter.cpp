/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "fpscounter.h"

FPSCounter::FPSCounter() : current(0), frameCount(0), totalElapsedTime(0) {
}

void FPSCounter::Update(float deltaTime) {
    totalElapsedTime += deltaTime;
    frameCount++;
    justUpdated = false;

    if (totalElapsedTime > 1.0f) {
        current = static_cast<float>(frameCount) / totalElapsedTime;

        frameCount = 0;
        totalElapsedTime = 0;
        justUpdated = true;
    }
}