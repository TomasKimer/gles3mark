

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "fpscounter.h"

class BenchmarkStatistics {
    friend class JSONStatsBuilder;

    unsigned score;
    float deltaBest, deltaWorst, deltaAvg, deltaStdDev, deltaAcc;
    float fpsBest, fpsWorst, fpsAvg, fpsStdDev, fpsAcc;

    std::vector<float> deltas;
    std::vector<float> fpss;

    bool running;

public:
    FPSCounter fpsCounter;

    BenchmarkStatistics() {
        Reset();
    }

    void Reset() {
        running = false;

        score = 0;
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
        
        deltaAcc += deltaTime;
        deltas.push_back(deltaTime);
            
        fpsCounter.Update(deltaTime);
        
        if (fpsCounter.JustUpdated()) {
            float fpsCurrent = fpsCounter.Current();

            if (fpsCurrent > fpsBest ) fpsBest  = fpsCurrent;
            if (fpsCurrent < fpsWorst) fpsWorst = fpsCurrent;

            fpsAcc += fpsCurrent;
            fpss.push_back(fpsCurrent);
        }
    }

    void EndMeasure() {
        running = false;
        
        score = deltas.size();
        
        deltaAvg = deltaAcc / deltas.size();
        deltaStdDev = ComputeStdDev(deltas, deltaAvg);
        
        fpsAvg   = fpsAcc / fpss.size();
        fpsStdDev = ComputeStdDev(fpss, fpsAvg);
    }

private:
    static float ComputeStdDev(const std::vector<float>& values, float mean) {
    
        float acc = 0.0f;

        for (unsigned i = 0; i < values.size(); ++i) {
            float diff = values[i] - mean;
            acc += diff * diff;
        }

        float variance = acc / values.size();
    
        if (variance != 0.0f)
            return glm::sqrt(variance);
        else
            return 0.0f;
    }
};