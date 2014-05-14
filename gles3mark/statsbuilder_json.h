

#pragma once

#include <string>
#include <iostream>  // fix of jsoncons lib error - std::cerr is not member of std
#include <jsoncons/json.hpp> // http://sourceforge.net/p/jsoncons/wiki/Home/

#include "benchmarkstatistics.h"

// http://cs.wikipedia.org/wiki/Builder
// http://developer.android.com/reference/android/os/StrictMode.ThreadPolicy.Builder.html
// JSONStats* st = (new JSONStatsBuilder()).BuildBenchStatsInfo().BuildGLinfo().Build();
class JSONStatsBuilder {
    jsoncons::json result;

    template <typename T>
    std::string toStr(T val) {
        std::stringstream ss;
        ss << val;
        return ss.str();    
    }

public:
    JSONStatsBuilder& BuildGLinfo();
    JSONStatsBuilder& BuildBenchStatsInfo(const BenchmarkStatistics& benchStats);

    std::string Build();
};