
from google.appengine.ext import ndb


class ResultItem(ndb.Model):
    score = ndb.IntegerProperty()
    device = ndb.StringProperty()

