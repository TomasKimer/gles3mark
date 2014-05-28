/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "fpscounter.h"

/**
 * \brief Benchmark statistics.
 */
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