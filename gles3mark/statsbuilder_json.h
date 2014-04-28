

#pragma once

#include <string>
#include <sstream>

#include "glquery.h"
#include "benchmarkstatistics.h"

// http://cs.wikipedia.org/wiki/Builder
class JSONStatsBuilder {
    std::stringstream result;

public:

    std::string GetResultJSON(const BenchmarkStatistics& benchStats) {
        BuildBenchStatsInfo(benchStats);
        
        return result.str();    
    }


    JSONStatsBuilder& BuildGLinfo() {
        result << GLQuery::RENDERER();

        return *this;
    }

    JSONStatsBuilder& BuildBenchStatsInfo(const BenchmarkStatistics& benchStats) {
        result << benchStats.score;
        
        return *this;
    }



};