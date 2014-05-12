
#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "fpscounter.h"

class BenchmarkStatistics {
    friend class JSONStatsBuilder;

    FPSCounter fpsCounter;

    unsigned score;
    float deltaBest, deltaWorst, deltaAvg, deltaStdDev, deltaAcc;
    float fpsBest, fpsWorst, fpsAvg, fpsStdDev, fpsAcc;
    bool running;

    std::vector<float> deltas;
    std::vector<float> fpss;

public:
    BenchmarkStatistics();

    void Reset();

    void StartMeasure();

    void OnFrame(float deltaTime);

    void EndMeasure();

    const FPSCounter& GetFPSCounter() { return fpsCounter; }

private:
    static float ComputeStdDev(const std::vector<float>& values, float mean);
};