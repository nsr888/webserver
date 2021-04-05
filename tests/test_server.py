import http.client
'''https://docs.python.org/3/library/http.client.html'''
import pytest
'''
https://habr.com/ru/company/yandex/blog/242795/
https://pytest-docs-ru.readthedocs.io/ru/latest/fixture.html
'''


@pytest.fixture
def connection(request):
    '''Prepair connection'''
    _connection = http.client.HTTPConnection("localhost", port=7777)

    def fin():
        _connection.close()
    request.addfinalizer(fin)
    return _connection


def test_connection(connection):
    '''Test responses'''
    connection.request("GET", "/")
    resp = connection.getresponse()
    assert "Hello world!" in resp.read().decode('utf-8')
    assert resp.getheader('Content-Type') == 'text/plain'
    assert resp.version == 11
    assert resp.status == 200
    assert resp.reason == 'OK'
