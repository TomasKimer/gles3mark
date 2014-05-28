/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "benchmarkstatistics.h"

BenchmarkStatistics::BenchmarkStatistics() {
    Reset();
}

void BenchmarkStatistics::Reset() {
    running = false;
    
    score = 0;
    deltaWorst = deltaAvg = deltaAcc = deltaStdDev = 0.0f;
    fpsBest = fpsAvg = fpsAcc = fpsStdDev = 0.0f;
    fpsWorst = deltaBest = 9999.0f;
}

void BenchmarkStatistics::StartMeasure() {
    running = true;
}

void BenchmarkStatistics::OnFrame(float deltaTime) {
    if (!running) return;
    
    deltaBest  = glm::min(deltaTime, deltaBest );
    deltaWorst = glm::max(deltaTime, deltaWorst);
    
    deltaAcc += deltaTime;
    deltas.push_back(deltaTime);
        
    fpsCounter.Update(deltaTime);
    
    if (fpsCounter.JustUpdated()) {
        float fpsCurrent = fpsCounter.Current();

        fpsBest  = glm::max(fpsCurrent, fpsBest );
        fpsWorst = glm::min(fpsCurrent, fpsWorst);

        fpsAcc += fpsCurrent;
        fpss.push_back(fpsCurrent);
    }
}

void BenchmarkStatistics::EndMeasure() {
    running = false;
    
    score = deltas.size();
    
    if (deltas.size() > 0) {
        deltaAvg = deltaAcc / deltas.size();
        deltaStdDev = ComputeStdDev(deltas, deltaAvg);
    }
    
    if (fpss.size() > 0) {
        fpsAvg   = fpsAcc / fpss.size();
        fpsStdDev = ComputeStdDev(fpss, fpsAvg);
    }
}

float BenchmarkStatistics::ComputeStdDev(const std::vector<float>& values, float mean) {
    float acc = 0.0f;

    for (unsigned i = 0; i < values.size(); ++i) {
        float diff = values[i] - mean;
        acc += diff * diff;
    }

    float variance = acc / values.size();

    return glm::sqrt(variance);
}