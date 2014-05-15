
#pragma once

#include <string>
#include <iostream>  // fix of jsoncons error - std::cerr is not member of std
#include <jsoncons/json.hpp> // http://sourceforge.net/p/jsoncons/wiki/Home/

#include "benchmarkstatistics.h"
#include "rendercontext.h"

class JSONStatsBuilder {
    jsoncons::json result;

    template <typename T>
    static std::string toStr(T val) {
        std::stringstream ss;
        ss << val;
        return ss.str();    
    }

public:
    JSONStatsBuilder& BuildGLinfo();
    JSONStatsBuilder& BuildGLContextInfo(const RenderContext& rc);
    JSONStatsBuilder& BuildBenchStatsInfo(const BenchmarkStatistics& benchStats);

    std::string Build();
};