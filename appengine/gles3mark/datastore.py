
from google.appengine.ext import ndb


class ResultItem(ndb.Model):
    score = ndb.IntegerProperty()
    device = ndb.StringProperty()
    renderer = ndb.StringProperty()

    uploader = ndb.StringProperty()
    date = ndb.DateTimeProperty(auto_now_add=True)

    benchInfo = ndb.TextProperty()
    glInfo = ndb.TextProperty()
    deviceInfo = ndb.TextProperty()

    #deviceInfoHW = ndb.TextProperty()
    #deviceInfoOS = ndb.StringProperty()
    #deviceInfoDisplay = ndb.StringProperty()
    #deviceInfoCPU = ndb.StringProperty()
    #deviceInfoMem = ndb.StringProperty()

