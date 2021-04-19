import http.client
'''https://docs.python.org/3/library/http.client.html'''
import pytest
'''
https://habr.com/ru/company/yandex/blog/242795/
https://pytest-docs-ru.readthedocs.io/ru/latest/fixture.html
'''
import urllib.parse


@pytest.fixture
def connection(request):
    '''Prepair connection'''
    _connection = http.client.HTTPConnection("localhost", port=7777)

    def fin():
        _connection.close()
    request.addfinalizer(fin)
    return _connection


def test_get_request(connection):
    '''Test get request'''
    connection.request("GET", "/")
    resp = connection.getresponse()
    assert "Hello world!" in resp.read().decode('utf-8')
    assert resp.getheader('Content-Type') == 'text/plain'
    assert resp.version == 11
    assert resp.status == 200
    assert resp.reason == 'OK'


def test_post_request(connection):
    '''Test post request'''
    params = urllib.parse.urlencode({'@key1': 'val', '@key2': 'va2'})
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    connection.request("POST", "/", params, headers)
    resp = connection.getresponse()
    assert "Hello world!" in resp.read().decode('utf-8')
    assert resp.getheader('Content-Type') == 'text/plain'
    assert resp.version == 11
    assert resp.status == 200
    assert resp.reason == 'OK'


def chunk_data(data, chunk_size):
    dl = len(data)
    ret = ""
    for i in range(dl // chunk_size):
        ret += "%s\r\n" % (hex(chunk_size)[2:])
        ret += "%s\r\n\r\n" % (data[i * chunk_size: (i + 1) * chunk_size])
    if len(data) % chunk_size != 0:
        ret += "%s\r\n" % (hex(len(data) % chunk_size)[2:])
        ret += "%s\r\n" % (data[-(len(data) % chunk_size):])

    ret += "0\r\n\r\n"
    return ret


def test_post_chunked_request(connection):
    '''Test post request'''
    connection.putrequest('POST', '/something')
    connection.putheader('Transfer-Encoding', 'chunked')
    connection.endheaders()
    params = urllib.parse.urlencode({'@key1': 'val', '@key2': 'va2'})
    connection.send(chunk_data(params, 3).encode('utf-8'))
    resp = connection.getresponse()
    assert "Hello world!" in resp.read().decode('utf-8')
    assert resp.getheader('Content-Type') == 'text/plain'
    assert resp.version == 11
    assert resp.status == 200
    assert resp.reason == 'OK'
