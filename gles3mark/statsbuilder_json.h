

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
        //result << 42;//GLQuery::RENDERER();
                // display some GL info
        //"GL_VENDOR: "                   + GLQuery::Get<std::string>(GL_VENDOR);
        //"GL_RENDERER: "                 + GLQuery::Get<std::string>(GL_RENDERER);
        //"GL_VERSION: "                  + GLQuery::Get<std::string>(GL_VERSION);
        //"GL_SHADING_LANGUAGE_VERSION: " + GLQuery::Get<std::string>(GL_SHADING_LANGUAGE_VERSION);

        //"Max render buffer size: " << GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE) << ", max samples: " << GLQuery::Get<GLint>(GL_MAX_SAMPLES);  // min 2048
        //"Max texture size: " << GLQuery::Get<GLint>(GL_MAX_TEXTURE_SIZE); // min 2048
        //std::vector<GLint> maxDims = GLQuery::Get<GLint>(GL_MAX_VIEWPORT_DIMS, 2);
        //"Max viewport dims: " << maxDims[0] << "x" << maxDims[1];
        //"Max color attachments: " << GLQuery::Get<GLint>(GL_MAX_COLOR_ATTACHMENTS); // min 4
        //"Max vertex attributes: " << GLQuery::Get<GLint>(GL_MAX_VERTEX_ATTRIBS); 

        return *this;
    }

    JSONStatsBuilder& BuildBenchStatsInfo(const BenchmarkStatistics& benchStats) {
        result << benchStats.score;
        
        return *this;
    }



};