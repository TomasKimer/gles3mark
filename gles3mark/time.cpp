
#include "time.h"

Time::Time() {
    Init();
}

void Time::Init() {
    start = previous = Clock::now();
}

void Time::Update() {
    current = Clock::now();
    elapsed = current - previous;
    //total = current - start; 
    previous = current;                
}

float Time::DeltaTime() const {
    return elapsed.count();
}

float Time::RealTimeSinceStartup() const {
    return std::chrono::duration<float>(current - start).count();
}