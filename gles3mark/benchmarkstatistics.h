

#pragma once

#include "fpscounter.h"

class BenchmarkStatistics {
    friend class JSONStatsBuilder;

    unsigned frameCount;
    float deltaBest, deltaWorst, deltaMean, deltaStdDev;
    float deltaAcc;
    float fpsBest, fpsWorst, fpsMean, fpsStdDev;

    unsigned score;
    bool running;

public:
    BenchmarkStatistics(): score(0) {    
    }

    void StartMeasure() {
        running = true;
        frameCount = 0;
        deltaAcc = 0.f;
    }


    void OnFrame(float deltaTime) {
        if (!running) return;

        deltaAcc += deltaTime;    
        frameCount++;
    }

    void EndMeasure() {
        running = false;
        
        score = frameCount;
        deltaMean = deltaAcc / frameCount;
    }
};