

#pragma once

#include <string>
#include <sstream>

#include "glquery.h"
#include "benchmarkstatistics.h"

// http://cs.wikipedia.org/wiki/Builder
// http://developer.android.com/reference/android/os/StrictMode.ThreadPolicy.Builder.html
// JSONStats* st = (new JSONStatsBuilder()).BuildBenchStatsInfo().BuildGLinfo().Build();
class JSONStatsBuilder {
    std::stringstream result;

public:

    std::string GetResultJSON(const BenchmarkStatistics& benchStats) {
        BuildBenchStatsInfo(benchStats);
        
        return result.str();    
    }

    std::string Build() {
        return result.str();
    }


    JSONStatsBuilder& BuildGLinfo() {
        result << 42;//GLQuery::RENDERER();

        return *this;
    }

    JSONStatsBuilder& BuildBenchStatsInfo(const BenchmarkStatistics& benchStats) {
        result << benchStats.score;
        
        return *this;
    }



};