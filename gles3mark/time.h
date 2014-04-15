

#pragma once

#include <chrono>
#include <ostream>
#include <iomanip>

#include "log.h"


// http://stackoverflow.com/questions/14391327/how-to-get-duration-as-int-millis-and-float-seconds-from-chrono

// TODO Diff BEST, WORST, AVG, like FPS (SPF is better metric)
class Time {
    typedef std::chrono::high_resolution_clock Clock;
    
public:
    Time() {
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
    }

    friend std::ostream& operator << (std::ostream& o, const Time& v) {
        o << "D: " << std::setw(3) << v.DeltaTime() * 1000 << " ms";
        return o;
    }

private:
    Clock::time_point start, current, previous;
    std::chrono::duration<float> elapsed; // total;
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