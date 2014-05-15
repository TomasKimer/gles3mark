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

from jinja import render_html
from datastore import ResultItem

def get_platform_css(user_agent):
    style = "main.css"
    if user_agent == "gles3mark_android_app":  # or user_agent.find("Android") != -1 or user_agent.find("Mobile") != -1:
        style = "androidapp.css"
    return style

class MainHandler(webapp2.RequestHandler):
    def get(self):
        results = ResultItem.query().order(-ResultItem.score).fetch()
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
        dbrec.deviceInfo = json.dumps(rec['DeviceInfo']);

        #dbrec.deviceInfoHW = json.dumps(rec['DeviceInfo']['HW']);
        #dbrec.deviceInfoOS = json.dumps(rec['DeviceInfo']['OS']);
        #dbrec.deviceInfoDisplay = json.dumps(rec['DeviceInfo']['Display']);
        #dbrec.deviceInfoCPU = json.dumps(rec['DeviceInfo']['CPU']);
        #dbrec.deviceInfoMem = json.dumps(rec['DeviceInfo']['Mem']);

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
                deviceInfo = json.loads(dbrec.deviceInfo)

                render_html(self, "detail.html", get_platform_css(self.request.headers['User-Agent']),
                            u"gles3mark result detail", u'',
                            template_values={"db": dbrec, "bench": benchinfo, "gl": glinfo, "device": deviceInfo})
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

        dbrec.score = 42
        dbrec.device = "GT-9505"
        dbrec.renderer = "Adreno(TM) 320"

        dbrec.uploader = "TooM"

        dbrec.benchInfo = "{}"
        dbrec.glInfo = "{}"
        dbrec.deviceInfo = "{}"

        dbrec.put()

        self.response.write("Setup done.")
        self.redirect('/')

app = webapp2.WSGIApplication([
    ('/', MainHandler),
    ('/detail', DetailHandler),
    ('/admin', AdminHandler),
    ('/setup', SetupHandler)
], debug=True)
