

#pragma once

#include <chrono>
#include <ostream>
#include <iomanip>

#include "log.h"


// http://stackoverflow.com/questions/14391327/how-to-get-duration-as-int-millis-and-float-seconds-from-chrono

class Time {
    typedef std::chrono::high_resolution_clock Clock;
    
public:
    Time(): average(0.f), best(9999.f), worst(0.f) {
        start = previous = Clock::now();
    }

    // The time in seconds it took to complete the last frame
    float DeltaTime() const {
        return elapsed.count();
    }

    // The real time in seconds since the game started.
    float RealTimeSinceStartup() const {
        return std::chrono::duration<float>(current - start).count();
    }

    void Update() {
        current = Clock::now();
        elapsed = current - previous;
        //total = current - start; 
        previous = current;                

        //if (RealTimeSinceStartup() > 1.5f) {
        //    fElapsed = elapsed.count();
        //    if (fElapsed < best ) best  = fElapsed;
        //    if (fElapsed > worst) worst = fElapsed;
        //    if (average == 0.0f)
        //            average = fElapsed;
        //        else
        //            average = (average + fElapsed) / 2.0f;
        //}       
    }

    friend std::ostream& operator << (std::ostream& o, const Time& v) {
        o <<   "C: " << std::setw(3) << std::fixed << std::setprecision(4) << v.DeltaTime() * 1000;
//        << ", A: " << std::setw(3) << std::fixed << std::setprecision(4) << v.average  * 1000
//        << ", B: " << std::setw(3) << v.best     * 1000
//        << ", W: " << std::setw(3) << v.worst    * 1000;
        return o;
    }

private:
    Clock::time_point start, current, previous;
    std::chrono::duration<float> elapsed; // total;
    float average, best, worst, fElapsed; // DeltaTime statictics
};



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

//StopWatch<std::chrono::high_resolution_clock> sw;
//std::this_thread::sleep_for(std::chrono::milliseconds(100));
//Log::V() << "Elapsed time: " << sw.elapsed<std::chrono::nanoseconds>() << " nanoseconds";