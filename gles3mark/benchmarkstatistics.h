

#pragma once



class BenchmarkStatistics {

    unsigned frameCount;
    float deltaBest, deltaWorst, deltaMean;
    bool running;

public:
    BenchmarkStatistics() {
    
    }

    void Start() {
        running = true;
        frameCount = 0;

    }


    void OnFrame(float deltaTime) {
        if (!running) return;
    
        frameCount++;
    }

    void End() {
        running = false;
    }

    unsigned GetFrameCount() {
        return frameCount;
    
    }
};