

#pragma once

#include "fpscounter.h"

class BenchmarkStatistics {
    friend class JSONStatsBuilder;

    unsigned score, frameCount, fpsCount;
    float deltaBest, deltaWorst, deltaAvg, deltaAcc, deltaStdDev;
    float fpsBest, fpsWorst, fpsAvg, fpsAcc, fpsStdDev;

    bool running;

public:
    FPSCounter fpsCounter;

    BenchmarkStatistics() {
        Reset();
    }

    void Reset() {
        running = false;

        score = frameCount = fpsCount = 0;
        deltaWorst = deltaAvg = deltaAcc = deltaStdDev = 0.0f;
        fpsBest = fpsAvg = fpsAcc = fpsStdDev = 0.0f;
        fpsWorst = deltaBest = 9999.0f;
    }

    void StartMeasure() {
        running = true;
    }

    void OnFrame(float deltaTime) {
        if (!running) return;

        if (deltaTime < deltaBest ) deltaBest  = deltaTime;
        if (deltaTime > deltaWorst) deltaWorst = deltaTime;
        if (deltaAvg == 0.0f)
            deltaAvg = deltaTime;
        else
            deltaAvg = (deltaAvg + deltaTime) / 2.0f;


        fpsCounter.Update(deltaTime);
        
        if (fpsCounter.JustUpdated()) {
            float fpsCurrent = fpsCounter.Current();

            if (fpsCurrent > fpsBest ) fpsBest  = fpsCurrent;
            if (fpsCurrent < fpsWorst) fpsWorst = fpsCurrent;

            if (fpsAvg == 0.0f)
                fpsAvg = fpsCurrent;
            else
                fpsAvg = (fpsAvg + fpsCurrent) / 2.0f;
        }

        deltaAcc += deltaTime;    
        frameCount++;
    }

    void EndMeasure() {
        running = false;
        
        score = frameCount;
        
        deltaAvg = deltaAcc / frameCount;
    }
};