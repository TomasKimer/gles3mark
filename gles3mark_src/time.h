

#pragma once

#include <chrono>
#include "log.h"





class Time {
public:


public:
    static void Test() {
        //typedef std::chrono::high_resolution_clock Clock;
        //auto t1 = Clock::now();
        //auto t2 = Clock::now();
        //Log::V() << t2 - t1;
    }

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