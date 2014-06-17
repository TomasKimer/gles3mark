
from google.appengine.ext import ndb


class ResultItem(ndb.Model):
    score = ndb.IntegerProperty()
    device = ndb.StringProperty()
    renderer = ndb.StringProperty()

    uploader = ndb.StringProperty()
    date = ndb.DateTimeProperty(auto_now_add=True)

    benchInfo = ndb.TextProperty()
    glInfo = ndb.TextProperty()
    glContextInfo = ndb.TextProperty()
    deviceInfo = ndb.TextProperty()


