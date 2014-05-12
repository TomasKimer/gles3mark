
#pragma once

#include <chrono>
#include <ostream>
#include <iomanip>

#include "log.h"

// http://stackoverflow.com/questions/14391327/how-to-get-duration-as-int-millis-and-float-seconds-from-chrono
class Time {
    typedef std::chrono::high_resolution_clock Clock;
    
public:
    Time();

    void Init();
    void Update();

    // The time in seconds it took to complete the last frame
    float DeltaTime() const;

    // The real time in seconds since the game started.
    float RealTimeSinceStartup() const;

    friend std::ostream& operator << (std::ostream& o, const Time& v) {
        o << std::setw(3) << std::fixed << std::setprecision(4) << v.DeltaTime() * 1000;
        return o;
    }

private:
    Clock::time_point start, current, previous;
    std::chrono::duration<float> elapsed; // total;
    float average, best, worst, fElapsed; // DeltaTime statictics
};


/*
 *  Generic StopWatch
 *  Usage:
 *      StopWatch<std::chrono::high_resolution_clock> sw;
 *      // stuff
 *      auto elapsed = sw.elapsed<std::chrono::nanoseconds>();
 */
template<typename C>
class StopWatch {
    std::chrono::time_point<C> start;
public:
    StopWatch() : start(C::now()) {}
    template<typename U>
    typename U::rep elapsed() const {
        return std::chrono::duration_cast<U>(C::now() - start).count();
    }
    void reset() {
        start = C::now();
    }
};