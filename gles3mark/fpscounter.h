

#pragma once

#include <ostream>
#include <iomanip>

class FPSCounter {
public:
    FPSCounter() :
        current(0), average(0), best(0), worst(9999.0f),
        totalFrames(0), frameCount(0), totalElapsedTime(0) {
    }

    void Update(float deltaTime) {
        totalElapsedTime += deltaTime;
        frameCount++;
        justUpdated = false;

        if (totalElapsedTime > 1.0f) {
            current = static_cast<float>(frameCount) / totalElapsedTime;

            if (current > best ) best  = current;
            if (current < worst) worst = current;

            if (average == 0.0f)
                average = current;
            else
                average = (average + current) / 2.0f;

            frameCount = 0;
            totalElapsedTime = 0;
            justUpdated = true;
        }

        totalFrames++;
    }

    float Current() const { return current; }
    float Average() const { return average; }
    float Best()    const { return best   ; }
    float Worst()   const { return worst  ; }
    unsigned int TotalFrames() const { return totalFrames; }
    float JustUpdated() const { return justUpdated; }

    friend std::ostream& operator << (std::ostream& o, const FPSCounter& v) {
        o <<   "C: " << std::setw(7) << v.Current();  // std::fixed << std::setprecision(4)
//        << ", A: " << std::setw(7) << v.Average();
//        << ", B: " << std::setw(7) << v.Best()
//        << ", W: " << std::setw(7) << v.Worst();
        return o;
    }

private:
    float current, average, best, worst;
    float totalElapsedTime;
    unsigned int frameCount, totalFrames;
    bool justUpdated;
};