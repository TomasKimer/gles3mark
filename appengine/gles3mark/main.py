#!/usr/bin/env python
#
# Copyright 2007 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import webapp2
import json
import collections

from jinja import render_html
from datastore import ResultItem

def get_platform_css(user_agent):
    style = "main.css"
    if user_agent == "gles3mark_android_app":  # or user_agent.find("Android") != -1 or user_agent.find("Mobile") != -1:
        style = "androidapp.css"
    return style

class MainHandler(webapp2.RequestHandler):
    def get(self):
        results = ResultItem.query().order(-ResultItem.score).fetch() #order(ResultItem.date).
        render_html(self, 'main.html', get_platform_css(self.request.headers['User-Agent']), u"gles3mark result database",
                    self.request.headers['User-Agent'], template_values={"results": results})

    def post(self):
        rec = json.loads(self.request.body)

        dbrec = ResultItem()

        dbrec.score = int(rec['BenchInfo']['score'])
        dbrec.device = rec['DeviceInfo']['HW']['Model']
        dbrec.renderer = rec['GLInfo']['Renderer']

        dbrec.uploader = rec['Uploader'];

        dbrec.benchInfo = json.dumps(rec['BenchInfo'])
        dbrec.glInfo = json.dumps(rec['GLInfo'])
        dbrec.glContextInfo = json.dumps(rec['GLContextInfo'])
        dbrec.deviceInfo = json.dumps(rec['DeviceInfo']);

        dbrec.put()

        self.response.out.write("received with thanks");

class DetailHandler(webapp2.RequestHandler):
    def get(self):
        idlist = self.request.arguments()
        if len(idlist) == 1:
            id = int(idlist[0])
            dbrec = ResultItem.get_by_id(id)
            if type(dbrec) is ResultItem:
                benchinfo = json.loads(dbrec.benchInfo)
                glinfo = json.loads(dbrec.glInfo)
                glcontext = json.loads(dbrec.glContextInfo)
                gllimits = collections.OrderedDict(sorted(glinfo['Limits'].items()))
                deviceInfo = json.loads(dbrec.deviceInfo)

                render_html(self, "detail.html", get_platform_css(self.request.headers['User-Agent']),
                            u"gles3mark result detail", u'',
                            template_values={
                                "db": dbrec,
                                "bench": benchinfo,
                                "gl": glinfo,
                                "gllimits": gllimits,
                                "glcontext": glcontext,
                                "device": deviceInfo})
            else:
                self.response.write("Result with this id does not exist")
        else:
            self.response.write("No id specified")

class AdminHandler(webapp2.RequestHandler):
    def get(self):
        self.response.write("Admin page is under construction.");

class SetupHandler(webapp2.RequestHandler):
    def get(self):
        dbrec = ResultItem()

        dbrec.score = 420
        dbrec.device = "GT-9505"
        dbrec.renderer = "Adreno (TM) 320"

        dbrec.uploader = "TooM"

        dbrec.benchInfo = json.dumps({u'SPFbest': u'181.824', u'score': 76, u'FPSavg': u'5.14924', u'FPSbest': u'5.45013', u'SPFavg': u'193.683', u'FPSworst': u'4.53536', u'FPSstddev': u'0.365979', u'SPFworst': u'249.725', u'SPFstddev': u'15.919'})
        dbrec.glInfo = json.dumps({u'Vendor': u'Qualcomm', u'Limits': {u'MIN PROGRAM TEXEL OFFSET': -8, u'MAX PROGRAM TEXEL OFFSET': 7, u'MAX VERTEX UNIFORM VECTORS': 256, u'MAX VERTEX UNIFORM BLOCKS': 12, u'NUM COMPRESSED TEXTURE FORMATS': 54, u'MAX ARRAY TEXTURE LAYERS': 256, u'MAX COMBINED UNIFORM BLOCKS': 24, u'SUBPIXEL BITS': 4, u'MAX UNIFORM BUFFER BINDINGS': 24, u'MAX SERVER WAIT TIMEOUT': u'1000000000', u'MAX 3D TEXTURE SIZE': 1024, u'MAX TRANSFORM FEEDBACK SEPARATE COMPONENTS': 4, u'MAX VERTEX TEXTURE IMAGE UNITS': 16, u'MAX TRANSFORM FEEDBACK INTERLEAVED COMPONENTS': 64, u'MAX COLOR ATTACHMENTS': 4, u'MAX SAMPLES': 4, u'UNIFORM BUFFER OFFSET ALIGNMENT': 4, u'MAX VERTEX ATTRIBS': 16, u'MAX ELEMENT INDEX': u'2147483647', u'MAX FRAGMENT UNIFORM COMPONENTS': 896, u'MAX CUBE MAP TEXTURE SIZE': 4096, u'MAX TEXTURE SIZE': 4096, u'MAX DRAW BUFFERS': 4, u'MAX TEXTURE IMAGE UNITS': 16, u'MAX UNIFORM BLOCK SIZE': u'65536', u'ALIASED POINT SIZE RANGE': u'1, 1023', u'MAX COMBINED VERTEX UNIFORM COMPONENTS': u'197632', u'MAX VIEWPORT DIMS': u'4096x4096', u'MAX VERTEX UNIFORM COMPONENTS': 1024, u'SAMPLES': 0, u'MAX VARYING VECTORS': 16, u'MAX COMBINED FRAGMENT UNIFORM COMPONENTS': u'197504', u'MAX TEXTURE LOD BIAS': u'15.9844', u'ALIASED LINE WIDTH RANGE': u'1, 8', u'MAX FRAGMENT INPUT COMPONENTS': 71, u'MAX FRAGMENT UNIFORM BLOCKS': 12, u'MAX VARYING COMPONENTS': 64, u'MAX VERTEX OUTPUT COMPONENTS': 69, u'MAX RENDERBUFFER SIZE': 4096, u'MAX COMBINED TEXTURE IMAGE UNITS': 32, u'MAX TRANSFORM FEEDBACK SEPARATE ATTRIBS': 4, u'MAX FRAGMENT UNIFORM VECTORS': 224}, u'Version': u'OpenGL ES 3.0 V@53.0 AU@ (CL@3776187)', u'Extensions': [u'GL_AMD_compressed_ATC_texture', u'GL_AMD_performance_monitor', u'GL_AMD_program_binary_Z400', u'GL_EXT_debug_label', u'GL_EXT_debug_marker', u'GL_EXT_discard_framebuffer', u'GL_EXT_robustness', u'GL_EXT_texture_format_BGRA8888', u'GL_EXT_texture_type_2_10_10_10_REV', u'GL_NV_fence', u'GL_OES_compressed_ETC1_RGB8_texture', u'GL_OES_depth_texture', u'GL_OES_depth24', u'GL_OES_EGL_image', u'GL_OES_EGL_image_external', u'GL_OES_element_index_uint', u'GL_OES_fbo_render_mipmap', u'GL_OES_fragment_precision_high', u'GL_OES_get_program_binary', u'GL_OES_packed_depth_stencil', u'GL_OES_depth_texture_cube_map', u'GL_OES_rgb8_rgba8', u'GL_OES_standard_derivatives', u'GL_OES_texture_3D', u'GL_OES_texture_float', u'GL_OES_texture_half_float', u'GL_OES_texture_half_float_linear', u'GL_OES_texture_npot', u'GL_OES_vertex_half_float', u'GL_OES_vertex_type_10_10_10_2', u'GL_OES_vertex_array_object', u'GL_QCOM_alpha_test', u'GL_QCOM_binning_control', u'GL_QCOM_driver_control', u'GL_QCOM_perfmon_global_mode', u'GL_QCOM_extended_get', u'GL_QCOM_extended_get2', u'GL_QCOM_tiled_rendering', u'GL_QCOM_writeonly_rendering', u'GL_EXT_sRGB', u'GL_EXT_texture_filter_anisotropic', u'GL_EXT_color_buffer_float', u'GL_EXT_color_buffer_half_float', u'GL_EXT_disjoint_timer_query'], u'Renderer': u'Adreno (TM) 320', u'GLSL version': u'OpenGL ES GLSL ES 3.00'}  )
        dbrec.glContextInfo = json.dumps({u'Min swap interval': 0, u'Version': u'1.4', u'Surface size': u'1920x1080', u'Max swap interval': 1})
        dbrec.deviceInfo = json.dumps({u'Mem': {u'Total memory': u'1808.8008 MB'}, u'HW': {u'Product': u'cm_jflte', u'Brand': u'samsung', u'Hardware': u'qcom', u'Board': u'MSM8960', u'Device': u'jfltexx', u'Serial': u'fe655af4', u'Model': u'GT-I9505', u'Id': u'KVT49L', u'Manufacturer': u'samsung'}, u'OS': {u'VM specification': u'Dalvik Virtual Machine Specification 0.9', u'Kernel': u'Linux version 3.4.0-cyanogenmod-g08b5664 (build01@cyanogenmod) (gcc version 4.7 (GCC) ) #1 SMP PREEMPT Sun May 4 15:52:26 PDT 2014', u'VM implementation': u'Dalvik 1.6.0', u'Tags': u'test-keys', u'Type': u'userdebug', u'Android SDK version': 19, u'Build ID': u'cm_jflte-userdebug 4.4.2 KVT49L 7c12a4e51d test-keys', u'Android version': u'4.4.2', u'Java specification': u'Dalvik Core Library 0.9', u'Fingerprint': u'samsung/jfltexx/jflte:4.2.2/JDQ39/I9505XXUAMDE:user/release-keys', u'Android version incremental': u'7c12a4e51d REL', u'Bootloader': u'I9505XXUBMF4', u'OS': u'Linux 3.4.0-cyanogenmod-g08b5664 armv7l'}, u'CPU': {u'ABI2': u'armeabi', u'Cores': 4, u'Freq': u'384 ~ 1890 MHz', u'ABI': u'armeabi-v7a'}, u'Display': {u'OpenGL ES version': u'3.0', u'Resolution': u'1920x1080', u'DPI': 480, u'Size': u'5inch'}})

        dbrec.put()

        self.response.write("Setup done.")
        self.redirect('/')

app = webapp2.WSGIApplication([
    ('/', MainHandler),
    ('/detail', DetailHandler),
    ('/admin', AdminHandler),
    ('/setup', SetupHandler)
], debug=True)
