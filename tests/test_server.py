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
