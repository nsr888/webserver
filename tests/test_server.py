import http.client
import pytest
import urllib.parse
import pprint
import requests


@pytest.fixture(scope="session")
def connection():
    _connection = http.client.HTTPConnection("localhost", port=8080,  timeout=10)
    yield _connection
    _connection.close()


# =============== good requests ===================

def test_get_request(connection):
    connection.request("GET", "/")
    resp = connection.getresponse()
    body = resp.read()
    assert "html" in body.decode('utf-8')
    assert 'text/html' in resp.getheader('Content-Type')
    assert resp.status == 200


def test_get_oulalala(connection):
    connection.request("GET", "/directory/oulalala")
    resp = connection.getresponse()
    resp.read()  # dont delete read, it important to call read()
    assert resp.status >= 400


def test_get_index_html_request(connection):
    connection.request("GET", "/index.html")
    resp = connection.getresponse()
    body = resp.read()
    assert "html" in body.decode('utf-8')
    assert 'text/html' in resp.getheader('Content-Type')
    assert resp.status == 200


def test_post_request(connection):
    params = urllib.parse.urlencode({'key1': 'val', 'key2': 'va2'})
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    connection.request("POST", "/post_body", params, headers)
    resp = connection.getresponse()
    body = resp.read()
    assert "ksinistr, ceccentr, resther" in body.decode('utf-8')
    assert resp.status == 200


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
    connection.putrequest('POST', '/post_body')
    connection.putheader('Transfer-Encoding', 'chunked')
    connection.endheaders()
    params = urllib.parse.urlencode({'key1': 'chunked', 'key2': 'CHUNKED'})
    connection.send(chunk_data(params, 3).encode('utf-8'))
    resp = connection.getresponse()
    body = resp.read()
    assert "ksinistr, ceccentr, resther" in body.decode('utf-8')
    # assert resp.getheader('Content-Type') == 'text/plain'
    assert resp.status == 200


def test_head_request(connection):
    connection.request("HEAD", "/head_test")
    resp = connection.getresponse()
    resp.read()
    assert resp.status == 200


def test_put_test_txt_request(connection):
    params = urllib.parse.urlencode({'key1': 'value1', 'key2': 'value2'})
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    connection.request("PUT", "/put_test/test.txt", params, headers)
    resp = connection.getresponse()
    resp.read()
    assert (resp.status in [200, 204, 210])


def test_get_test_txt_request(connection):
    connection.request("GET", "/get_test/put_test/test.txt")
    resp = connection.getresponse()
    resp.read()
    assert 'text/plain' in resp.getheader('Content-Type')
    assert resp.status == 200


# =============== wrong requests ===================

def test_wrong_method_request(connection):
    connection.request("GETZ", "/")
    resp = connection.getresponse()
    resp.read()  # dont delete read, it important to call read()
    assert resp.status >= 400


def test_wrong_target_request(connection):
    connection.request("GET", "/^")
    resp = connection.getresponse()
    resp.read()  # dont delete read, it important to call read()
    assert resp.status >= 400


def test_post_wrong_target_request(connection):
    params = urllib.parse.urlencode({'key1': 'val', 'key2': 'va2'})
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    connection.request("POST", "/onhuneothunheo", params, headers)
    resp = connection.getresponse()
    resp.read()  # dont delete read, it important to call read()
    assert resp.status >= 400


def test_post_wrong_request(connection):
    connection.request("POST", "/onhuneothunheo")
    resp = connection.getresponse()
    resp.read()  # dont delete read, it important to call read()
    assert resp.status >= 400


def test_head_wrong_request(connection):
    connection.request("HEAD", "/")
    resp = connection.getresponse()
    resp.read()  # dont delete read, it important to call read()
    assert resp.version == 11
    assert resp.status >= 400
