/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include <sstream>

#include "statsbuilder_json.h"
#include "glquery.h"

std::string JSONStatsBuilder::Build() {
    return result.as<std::string>();
}

JSONStatsBuilder& JSONStatsBuilder::BuildGLContextInfo(const RenderContext& rc) {
    jsoncons::json glContextJson;
    
    glContextJson["Version"] = toStr(rc.GetVersionMajor()) + "." + toStr(rc.GetVersionMinor());
    glContextJson["Surface size"] = toStr(rc.GetWidth()) + "x" + toStr(rc.GetHeight());
    glContextJson["Min swap interval"] = rc.GetSwapIntervalMin();
    glContextJson["Max swap interval"] = rc.GetSwapIntervalMax();

    result["GLContextInfo"] = glContextJson;

    return *this;
}

JSONStatsBuilder& JSONStatsBuilder::BuildGLinfo() {
    jsoncons::json glJson;
    
    glJson["Vendor"      ] = GLQuery::Get<std::string>(GL_VENDOR);
    glJson["Renderer"    ] = GLQuery::Get<std::string>(GL_RENDERER);
    glJson["Version"     ] = GLQuery::Get<std::string>(GL_VERSION);
    glJson["GLSL version"] = GLQuery::Get<std::string>(GL_SHADING_LANGUAGE_VERSION);
    
    jsoncons::json limits;

    std::vector<GLint>   maxViewportDims       = GLQuery::Get<GLint>(GL_MAX_VIEWPORT_DIMS, 2);
    std::vector<GLfloat> aliasedPointSizeRange = GLQuery::Get<GLfloat>(GL_ALIASED_POINT_SIZE_RANGE, 2);
    std::vector<GLfloat> aliasedLineWidthRange = GLQuery::Get<GLfloat>(GL_ALIASED_LINE_WIDTH_RANGE, 2);

    // -- GLint --
    limits["MAX VIEWPORT DIMS"               ] = toStr(maxViewportDims[0]) + "x" + toStr(maxViewportDims[1]); // Dimensions of the maximum supported viewport size
    limits["MAX TEXTURE SIZE"                ] = GLQuery::Get<GLint>(GL_MAX_TEXTURE_SIZE                   ); // Maximum size of a texture (min 2048)
    limits["MAX 3D TEXTURE SIZE"             ] = GLQuery::Get<GLint>(GL_MAX_3D_TEXTURE_SIZE                ); // Maximum size of 3D texture supported (min 256)
    limits["MAX RENDERBUFFER SIZE"           ] = GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE              ); // Maximum width and height of renderbuffers supported (min 2048)
    limits["MAX SAMPLES"                     ] = GLQuery::Get<GLint>(GL_MAX_SAMPLES                        ); // Maximum number of samples supported for multisampling (min 4)
    limits["MAX COLOR ATTACHMENTS"           ] = GLQuery::Get<GLint>(GL_MAX_COLOR_ATTACHMENTS              ); // Maximum number of color attachments supported (min 4)
    limits["MAX DRAW BUFFERS"                ] = GLQuery::Get<GLint>(GL_MAX_DRAW_BUFFERS                   ); // Maximum active number of draw buffers supported (min 4)
    limits["MAX VERTEX ATTRIBS"              ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_ATTRIBS                 ); // Maximum number of vertex attributes supported (min 16)
    limits["SUBPIXEL BITS"                   ] = GLQuery::Get<GLint>(GL_SUBPIXEL_BITS                      ); // Number of subpixel bits supported (min 4)  
    limits["MAX ARRAY TEXTURE LAYERS"        ] = GLQuery::Get<GLint>(GL_MAX_ARRAY_TEXTURE_LAYERS           ); // Maximum number of texture layers supported (min 256)
    limits["MAX CUBE MAP TEXTURE SIZE"       ] = GLQuery::Get<GLint>(GL_MAX_CUBE_MAP_TEXTURE_SIZE          ); // Maximum dimension of a cubemap texture (min 2048)
    limits["NUM COMPRESSED TEXTURE FORMATS"  ] = GLQuery::Get<GLint>(GL_NUM_COMPRESSED_TEXTURE_FORMATS     ); // Number of compressed texture formats supported (min 10)
    limits["MAX VERTEX UNIFORM COMPONENTS"   ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_UNIFORM_COMPONENTS      ); // Maximum number of components for vertex shader uniform variables supported (min 1024)
    limits["MAX VERTEX UNIFORM VECTORS"      ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_UNIFORM_VECTORS         ); // Maximum number of vectors for vertex shader uniform variables supported (min 256) 
    limits["MAX VERTEX UNIFORM BLOCKS"       ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_UNIFORM_BLOCKS          ); // Maximum number of vertex uniform buffers per program supported (min 12) 
    limits["MAX VERTEX OUTPUT COMPONENTS"    ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_OUTPUT_COMPONENTS       ); // Maximum number of components of outputs written by a vertex shader supported (min 64) 
    limits["MAX VERTEX TEXTURE IMAGE UNITS"  ] = GLQuery::Get<GLint>(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS     ); // Maximum number of texture image units accessible by a vertex shader supported (min 16) 
    limits["MAX FRAGMENT UNIFORM COMPONENTS" ] = GLQuery::Get<GLint>(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS    ); // Maximum number of components for fragment shader uniform variables supported (min 896) 
    limits["MAX FRAGMENT UNIFORM VECTORS"    ] = GLQuery::Get<GLint>(GL_MAX_FRAGMENT_UNIFORM_VECTORS       ); // Maximum number of vectors for fragment shader uniform variables supported (min 224) 
    limits["MAX FRAGMENT UNIFORM BLOCKS"     ] = GLQuery::Get<GLint>(GL_MAX_FRAGMENT_UNIFORM_BLOCKS        ); // Maximum number of fragment uniform buffers per program supported (min 12) 
    limits["MAX FRAGMENT INPUT COMPONENTS"   ] = GLQuery::Get<GLint>(GL_MAX_FRAGMENT_INPUT_COMPONENTS      ); // Maximum number of components of inputs read by a fragment shader supported (min 60) 
    limits["MAX TEXTURE IMAGE UNITS"         ] = GLQuery::Get<GLint>(GL_MAX_TEXTURE_IMAGE_UNITS            ); // Maximum number of texture image units accessible by a fragment shader supported (min 16) 
    limits["MIN PROGRAM TEXEL OFFSET"        ] = GLQuery::Get<GLint>(GL_MIN_PROGRAM_TEXEL_OFFSET           ); // Minimum texel offset allowed in a lookup supported (min –8) 
    limits["MAX PROGRAM TEXEL OFFSET"        ] = GLQuery::Get<GLint>(GL_MAX_PROGRAM_TEXEL_OFFSET           ); // Maximum texel offset allowed in a lookup supported (min 7) 
    limits["MAX UNIFORM BUFFER BINDINGS"     ] = GLQuery::Get<GLint>(GL_MAX_UNIFORM_BUFFER_BINDINGS        ); // Maximum number of uniform buffer bindings supported (min 24)  
    limits["UNIFORM BUFFER OFFSET ALIGNMENT" ] = GLQuery::Get<GLint>(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT    ); // Minimum required alignment for uniform buffer sizes and offsets supported (min 1 ) 
    limits["MAX COMBINED UNIFORM BLOCKS"     ] = GLQuery::Get<GLint>(GL_MAX_COMBINED_UNIFORM_BLOCKS        ); // Maximum number of uniform buffers per program supported (min 24) 
    limits["MAX VARYING COMPONENTS"          ] = GLQuery::Get<GLint>(GL_MAX_VARYING_COMPONENTS             ); // Maximum number of components for output variables supported (min 60) 
    limits["MAX VARYING VECTORS"             ] = GLQuery::Get<GLint>(GL_MAX_VARYING_VECTORS                ); // Maximum number of vectors for output variables supported (min 15) 
    limits["MAX COMBINED TEXTURE IMAGE UNITS"] = GLQuery::Get<GLint>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS   ); // Maximum number of accessible texture units supported (min 32) 
    limits["SAMPLES"                         ] = GLQuery::Get<GLint>(GL_SAMPLES                            ); // Coverage mask size (min 0) 

    limits["MAX TRANSFORM FEEDBACK INTERLEAVED COMPONENTS"] = GLQuery::Get<GLint>(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS); // Maximum number of components in interleaved mode supported (min 64) 
    limits["MAX TRANSFORM FEEDBACK SEPARATE COMPONENTS"   ] = GLQuery::Get<GLint>(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS   ); // Maximum number of components in separate mode supported (min 4) 
    limits["MAX TRANSFORM FEEDBACK SEPARATE ATTRIBS"      ] = GLQuery::Get<GLint>(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS      ); // Maximum number of separate attributes that can be captured in transform feedback supported (min 4) 

    // -- GLint64 --
    limits["MAX ELEMENT INDEX"                            ] = toStr(GLQuery::Get<GLint64>(GL_MAX_ELEMENT_INDEX                       )); // Maximum element index (min 2^24–1)
    limits["MAX SERVER WAIT TIMEOUT"                      ] = toStr(GLQuery::Get<GLint64>(GL_MAX_SERVER_WAIT_TIMEOUT                 )); // Maximum glWaitSync timeout interval (min 0)
    limits["MAX UNIFORM BLOCK SIZE"                       ] = toStr(GLQuery::Get<GLint64>(GL_MAX_UNIFORM_BLOCK_SIZE                  )); // Maximum size of a uniform block supported (min 16384)
    limits["MAX COMBINED VERTEX UNIFORM COMPONENTS"       ] = toStr(GLQuery::Get<GLint64>(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS  )); // Maximum number of words for vertex shader uniform variables in all uniform blocks supported     
    limits["MAX COMBINED FRAGMENT UNIFORM COMPONENTS"     ] = toStr(GLQuery::Get<GLint64>(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS)); // Maximum number of words for vertex shader uniform variables in all uniform blocks supported 

    // -- GLfloat --
    limits["ALIASED POINT SIZE RANGE"                     ] = toStr(aliasedPointSizeRange[0]) + ", " + toStr(aliasedPointSizeRange[1] ); // Range of aliased point sizes (min 1, 1)
    limits["ALIASED LINE WIDTH RANGE"                     ] = toStr(aliasedLineWidthRange[0]) + ", " + toStr(aliasedLineWidthRange[1] ); // Range of aliased line width sizes (min 1, 1)
    limits["MAX TEXTURE LOD BIAS"                         ] = toStr(GLQuery::Get<GLfloat>(GL_MAX_TEXTURE_LOD_BIAS                   ) ); // Maximum absolute texture level of detail bias supported (min 2.0)
    
    // -- unused --
//  limits["COMPRESSED_TEXTURE_FORMATS"                   ] = GLQuery::Get<GLint>(GL_COMPRESSED_TEXTURE_FORMATS      ); // Compressed texture formats supported                                         
//  limits["NUM_PROGRAM_BINARY_FORMATS"                   ] = GLQuery::Get<GLint>(GL_NUM_PROGRAM_BINARY_FORMATS      ); // Number of program binary formats supported (min 0)
//  limits["PROGRAM_BINARY_FORMATS"                       ] = GLQuery::Get<GLint>(GL_PROGRAM_BINARY_FORMATS          ); // Program binary formats supported                                                                        
//  limits["NUM_SHADER_BINARY_FORMATS"                    ] = GLQuery::Get<GLint>(GL_NUM_SHADER_BINARY_FORMATS       ); // Number of shader binary formats supported (min 0) 
//  limits["SHADER_BINARY_FORMATS"                        ] = GLQuery::Get<GLint>(GL_SHADER_BINARY_FORMATS           ); // Shader binary formats supported                                                                      
//  limits["IMPLEMENTATION_COLOR_READ_TYPE"               ] = GLQuery::Get<GLint>(GL_IMPLEMENTATION_COLOR_READ_TYPE  ); // Data type for pixel components for pixel read operations                                    
//  limits["IMPLEMENTATION_COLOR_READ_FORMAT"             ] = GLQuery::Get<GLint>(GL_IMPLEMENTATION_COLOR_READ_FORMAT); // Pixel format for pixel read operations                                                      

    // -- not found --
//  limits["MAX_ELEMENT_INDICES"                          ] = GLQuery::Get<GLint>(GL_MAX_ELEMENT_INDICES ); // Maximum number of glDrawRangeElements indices supported                                  
//  limits["MAX_ELEMENT_VERTICES"                         ] = GLQuery::Get<GLint>(GL_MAX_ELEMENT_VERTICES); // Maximum number of glDrawRangeElements vertices supported                                 
//  limits["SAMPLE_BUFFER"                                ] = GLQuery::Get<GLint>(GL_SAMPLE_BUFFER       ); // Number of multisample buffers (min 0)
    
    glJson["Limits"] = limits;

    std::vector<std::string> extensions = GLQuery::Extensions();
    jsoncons::json arr(extensions.begin(), extensions.end());
    glJson["Extensions"] = arr;

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