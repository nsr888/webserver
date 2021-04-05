'''https://docs.python.org/3/library/http.client.html'''
import pprint
import http.client

connection = http.client.HTTPConnection("localhost", port=7777)
connection.request("GET", (
    "/eonuheonuhneouhneohunoeuhn"
    "eouoeuuoueouoeueouneohunhuoenhunoehuneohununeounoehun"
    "eouoeuuoueouoeueouneohunhuoenhunoehuneohununeounoehun"
    "eouoeuuoueouoeueouneohunhuoenhunoehuneohununeounoehun"
    "eouoeuuoueouoeueouneohunhuoenhunoehuneohununeounoehun"
    "eouoeuuoueouoeueouneohunhuoenhunoehuneohununeounoehun"
    "eounhoenuoeu"))
response = connection.getresponse()
headers = response.getheaders()
pp = pprint.PrettyPrinter(indent=4)
pp.pprint("Headers: {}".format(headers))
body = response.read()
pp.pprint("Body: {}".format(body))
