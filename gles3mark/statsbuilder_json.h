

#pragma once

#include <string>
#include <sstream>

#include <iostream>  // fix of jsoncons lib error - std::cerr is not member of std
#include <jsoncons/json.hpp> // http://sourceforge.net/p/jsoncons/wiki/Home/

#include "glquery.h"
#include "benchmarkstatistics.h"

// http://cs.wikipedia.org/wiki/Builder
// http://developer.android.com/reference/android/os/StrictMode.ThreadPolicy.Builder.html
// JSONStats* st = (new JSONStatsBuilder()).BuildBenchStatsInfo().BuildGLinfo().Build();
class JSONStatsBuilder {
    jsoncons::json result;

public:

    std::string Build() {
        return result.as<std::string>();
    }

    JSONStatsBuilder& BuildGLinfo() {
        jsoncons::json glJson;
        
        glJson["Vendor"      ] = GLQuery::Get<std::string>(GL_VENDOR);
        glJson["Renderer"    ] = GLQuery::Get<std::string>(GL_RENDERER);
        glJson["Version"     ] = GLQuery::Get<std::string>(GL_VERSION);
        glJson["GLSL version"] = GLQuery::Get<std::string>(GL_SHADING_LANGUAGE_VERSION);

        //"Max render buffer size: " << GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE) << ", max samples: " << GLQuery::Get<GLint>(GL_MAX_SAMPLES);  // min 2048
        //"Max texture size: " << GLQuery::Get<GLint>(GL_MAX_TEXTURE_SIZE); // min 2048
        //std::vector<GLint> maxDims = GLQuery::Get<GLint>(GL_MAX_VIEWPORT_DIMS, 2);
        //"Max viewport dims: " << maxDims[0] << "x" << maxDims[1];
        //"Max color attachments: " << GLQuery::Get<GLint>(GL_MAX_COLOR_ATTACHMENTS); // min 4
        //"Max vertex attributes: " << GLQuery::Get<GLint>(GL_MAX_VERTEX_ATTRIBS); 

        result["GLInfo"] = glJson;

        return *this;
    }

    JSONStatsBuilder& BuildBenchStatsInfo(const BenchmarkStatistics& benchStats) {
        jsoncons::json benchJson;
        
        benchJson["score"] = benchStats.score;
        
        benchJson["FPSavg"   ] = toStr(benchStats.fpsAvg   );
        benchJson["FPSstddev"] = toStr(benchStats.fpsStdDev);
        benchJson["FPSbest"  ] = toStr(benchStats.fpsBest  );
        benchJson["FPSworst" ] = toStr(benchStats.fpsWorst );

        benchJson["SPFavg"   ] = toStr(benchStats.deltaAvg    * 1000.0f);
        benchJson["SPFstddev"] = toStr(benchStats.deltaStdDev * 1000.0f);
        benchJson["SPFbest"  ] = toStr(benchStats.deltaBest   * 1000.0f);
        benchJson["SPFworst" ] = toStr(benchStats.deltaWorst  * 1000.0f);
        
        result["BenchInfo"] = benchJson;
        
        return *this;
    }

private:
    std::string toStr(float val) {
        std::stringstream ss;
        ss << val;
        return ss.str();    
    }
};