
from google.appengine.ext import ndb


class ResultItem(ndb.Model):
    score = ndb.IntegerProperty()
    device = ndb.StringProperty()
    uploader = ndb.StringProperty()
    benchInfo = ndb.TextProperty()
    glInfo = ndb.TextProperty()
    deviceInfo = ndb.TextProperty()
    #deviceInfoHW = ndb.TextProperty()
    #deviceInfoOS = ndb.StringProperty()
    #deviceInfoDisplay = ndb.StringProperty()
    #deviceInfoCPU = ndb.StringProperty()
    #deviceInfoMem = ndb.StringProperty()

