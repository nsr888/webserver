'''https://docs.python.org/3/library/http.client.html'''
import pprint
import http.client
import urllib.parse

connection = http.client.HTTPConnection("localhost", port=7777)
params = urllib.parse.urlencode({'@key1': 'value1', '@key2': 'value2!!!!'})
headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
connection.request("POST", "/echo/post/form", params, headers)
response = connection.getresponse()
headers = response.getheaders()
pp = pprint.PrettyPrinter(indent=4)
pp.pprint("Headers: {}".format(headers))
body = response.read()
pp.pprint("Body: {}".format(body))
