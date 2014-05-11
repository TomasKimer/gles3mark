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

class MainHandler(webapp2.RequestHandler):
    def get(self):
        style = "main.css"
        usrAg = self.request.headers['User-Agent']
        if usrAg == "gles3mark_android_app" or usrAg.find("Android") != -1 or usrAg.find("Mobile") != -1:
            style = "androidapp.css"
        render_html(self, "list.html", style, u"gles3mark results database", self.request.headers['User-Agent']);
        #  self.response.write('Hello world from PyCharm2!')

    def post(self):
        req = self.request
        a = req.body
        b = json.loads(a)

        #self.response.out.write(b);
        self.response.out.write("Data received - score: " + b['score'] + ", info: " + b['info']);

app = webapp2.WSGIApplication([
    ('/', MainHandler)
], debug=True)
