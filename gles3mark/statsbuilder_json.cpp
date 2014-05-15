
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


    glJson["MAX_TEXTURE_SIZE"     ] = GLQuery::Get<GLint>(GL_MAX_TEXTURE_SIZE);      // Maximum size of a texture (min 2048)
    //GL_MAX_3D_TEXTURE_SIZE Maximum size of 3D texture supported (min 256) glGetIntegerv
    
    glJson["MAX_RENDERBUFFER_SIZE"] = GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE); // Maximum width and height of renderbuffers supported (min 2048)
    
    std::vector<GLint> maxDims = GLQuery::Get<GLint>(GL_MAX_VIEWPORT_DIMS, 2);
    glJson["MAX_VIEWPORT_DIMS"    ] = toStr(maxDims[0]) + "x" + toStr(maxDims[1]);   // Dimensions of the maximum supported viewport size
    
    glJson["MAX_SAMPLES"          ] = GLQuery::Get<GLint>(GL_MAX_SAMPLES);           // Maximum number of samples supported for multisampling (min 4)
    glJson["MAX_COLOR_ATTACHMENTS"] = GLQuery::Get<GLint>(GL_MAX_COLOR_ATTACHMENTS); // Maximum number of color attachments supported (min 4)
    glJson["MAX_DRAW_BUFFERS"     ] = GLQuery::Get<GLint>(GL_MAX_DRAW_BUFFERS);      // Maximum active number of draw buffers supported (min 4)

    glJson["MAX_VERTEX_ATTRIBS"   ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_ATTRIBS);    // Maximum number of vertex attributes supported (min 16)

/*
GL_MAX_ELEMENT_INDEX Maximum element index 2 24 – 1 glGetInteger64v
GL_SUBPIXEL_BITS Number of subpixel bits supported 4 glGetIntegerv   
GL_MAX_ARRAY_TEXTURE_LAYERS Maximum number of texture layers supported 256 glGetIntegerv
GL_MAX_TEXTURE_LOD_BIAS Maximum absolute texture level of detail bias supported 2.0 glGetFloatv
GL_MAX_CUBE_MAP_TEXTURE_SIZE Maximum dimension of a cubemap texture 2048 glGetIntegerv

GL_ALIASED_POINT_SIZE_RANGE Range of aliased point sizes 1, 1 glGetFloatv 
GL_ALIASED_LINE_WIDTH_RANGE Range of aliased line width sizes 1, 1 glGetFloatv
GL_MAX_ELEMENT_INDICES Maximum number of glDrawRangeElements indices supported glGetIntegerv
GL_MAX_ELEMENT_VERTICES Maximum number of glDrawRangeElements vertices supported glGetIntegerv
GL_NUM_COMPRESSED_TEXTURE_FORMATS Number of compressed texture formats supported 10 glGetIntegerv
GL_COMPRESSED_TEXTURE_FORMATS Compressed texture formats supported glGetIntegerv

GL_NUM_PROGRAM_BINARY_FORMATS Number of program binary formats supported 0 glGetIntegerv
GL_PROGRAM_BINARY_FORMATS Program binary formats supported glGetIntegerv
GL_NUM_SHADER_BINARY_FORMATS Number of shader binary formats supported 0 glGetIntegerv
GL_SHADER_BINARY_FORMATS Shader binary formats supported glGetIntegerv
GL_MAX_SERVER_WAIT_TIMEOUT Maximum glWaitSync timeout interval 0 glGetInteger64v

//GL_MAX_VERTEX_ATTRIBS Maximum number of vertex attributes supported 16 glGetIntegerv
GL_MAX_VERTEX_UNIFORM_COMPONENTS Maximum number of components for vertex shader uniform variables supported 1024 glGetIntegerv
GL_MAX_VERTEX_UNIFORM_VECTORS Maximum number of vectors for vertex shader uniform variables supported 256 glGetIntegerv
GL_MAX_VERTEX_UNIFORM_BLOCKS Maximum number of vertex uniform buffers per program supported 12 glGetIntegerv
GL_MAX_VERTEX_OUTPUT_COMPONENTS Maximum number of components of outputs written by a vertex shader supported 64 glGetIntegerv
GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS Maximum number of texture image units accessible by a vertex shader supported 16 glGetIntegerv

GL_MAX_FRAGMENT_UNIFORM_COMPONENTS Maximum number of components for fragment shader uniform variables supported 896 glGetIntegerv
GL_MAX_FRAGMENT_UNIFORM_VECTORS Maximum number of vectors for fragment shader uniform variables supported 224 glGetIntegerv
GL_MAX_FRAGMENT_UNIFORM_BLOCKS Maximum number of fragment uniform buffers per program supported 12 glGetIntegerv
GL_MAX_FRAGMENT_INPUT_COMPONENTS Maximum number of components of inputs read by a fragment shader supported 60 glGetIntegerv
GL_MAX_TEXTURE_IMAGE_UNITS Maximum number of texture image units accessible by a fragment shader supported 16 glGetIntegerv
GL_MIN_PROGRAM_TEXEL_OFFSET Minimum texel offset allowed in a lookup supported –8 glGetIntegerv
GL_MAX_PROGRAM_TEXEL_OFFSET Maximum texel offset allowed in a lookup supported 7 glGetIntegerv

GL_MAX_UNIFORM_BUFFER_BINDINGS Maximum number of uniform buffer bindings supported 24 glGetIntegerv
GL_MAX_UNIFORM_BLOCK_SIZE Maximum size of a uniform block supported 16384 glGetInteger64v
GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT Minimum required alignment for uniform buffer sizes and offsets supported 1 glGetIntegerv
GL_MAX_COMBINED_UNIFORM_BLOCKS Maximum number of uniform buffers per program supported 24 glGetIntegerv
GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS Maximum number of words for vertex shader uniform variables in all uniform blocks supported glGetInteger64v
GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS Maximum number of words for vertex shader uniform variables in all uniform blocks supported glGetInteger64v

GL_MAX_VARYING_COMPONENTS Maximum number of components for output variables supported 60 glGetIntegerv
GL_MAX_VARYING_VECTORS Maximum number of vectors for output variables supported 15 glGetIntegerv 
GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS Maximum number of accessible texture units supported 32 glGetIntegerv

GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS Maximum number of components in interleaved mode supported 64 glGetIntegerv
GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS Maximum number of components in separate mode supported 4 glGetIntegerv
GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS Maximum number of separate attributes that can be captured in transform feedback supported 4 glGetIntegerv

GL_SAMPLE_BUFFER Number of multisample buffers 0 glGetIntegerv 
GL_SAMPLES Coverage mask size 0 glGetIntegerv

GL_IMPLEMENTATION_COLOR_READ_TYPE Data type for pixel components for pixel read operations glGetIntegerv
GL_IMPLEMENTATION_COLOR_READ_FORMAT Pixel format for pixel read operations glGetIntegerv

*/

    
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
    
    benchJson["SPFavg"   ] = toStr(benchStats.deltaAvg    * 1000.0f);
    benchJson["SPFbest"  ] = toStr(benchStats.deltaBest   * 1000.0f);
    benchJson["SPFworst" ] = toStr(benchStats.deltaWorst  * 1000.0f);
    benchJson["SPFstddev"] = toStr(benchStats.deltaStdDev * 1000.0f);
    
    benchJson["FPSavg"   ] = toStr(benchStats.fpsAvg   );
    benchJson["FPSbest"  ] = toStr(benchStats.fpsBest  );
    benchJson["FPSworst" ] = toStr(benchStats.fpsWorst );
    benchJson["FPSstddev"] = toStr(benchStats.fpsStdDev);    
    
    result["BenchInfo"] = benchJson;
    
    return *this;
}