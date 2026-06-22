
import urllib.request
import urllib.parse

url = "http://localhost:8081/"
data = urllib.parse.urlencode({"content": "<script>alert(42)</script>"}).encode()

req = urllib.request.Request(url, data=data)
response = urllib.request.urlopen(req)

