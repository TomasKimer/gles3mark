
#include <sstream>

#include "statsbuilder_json.h"
#include "glquery.h"

std::string JSONStatsBuilder::Build() {
    return result.as<std::string>();
}

JSONStatsBuilder& JSONStatsBuilder::BuildGLinfo() {
    jsoncons::json glJson;
    
    glJson["Vendor"      ] = GLQuery::Get<std::string>(GL_VENDOR);
    glJson["Renderer"    ] = GLQuery::Get<std::string>(GL_RENDERER);
    glJson["Version"     ] = GLQuery::Get<std::string>(GL_VERSION);
    glJson["GLSL version"] = GLQuery::Get<std::string>(GL_SHADING_LANGUAGE_VERSION);

    std::vector<GLint> maxDims = GLQuery::Get<GLint>(GL_MAX_VIEWPORT_DIMS, 2);
    
    glJson["Max render buffer size"] = GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE);
    glJson["Max texture size"      ] = GLQuery::Get<GLint>(GL_MAX_TEXTURE_SIZE);
    glJson["Max viewport dims"     ] = toStr(maxDims[0]) + "x" + toStr(maxDims[1]);
    glJson["Max samples"           ] = GLQuery::Get<GLint>(GL_MAX_SAMPLES);
    glJson["Max color attachments" ] = GLQuery::Get<GLint>(GL_MAX_COLOR_ATTACHMENTS);
    glJson["Max vertex attributes" ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_ATTRIBS);

    
    // TODO JSONArray? or split ' '
    std::vector<std::string> extensions = GLQuery::Extensions();
    std::stringstream ss;
    for (const std::string& e : extensions) {
        ss << e << " ";
    }
    glJson["Extensions"] = ss.str();


    result["GLInfo"] = glJson;

    return *this;
}

JSONStatsBuilder& JSONStatsBuilder::BuildBenchStatsInfo(const BenchmarkStatistics& benchStats) {
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