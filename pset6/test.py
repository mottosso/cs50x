"""A testing-module for server.c"""

import requests


def test_ideal_case():
    r = requests.get("http://127.0.0.1:8080/cat.html")
    assert r.status_code == 200, r.status_code
    assert r.headers["connection"] == "close"
    assert r.headers["content-type"] == "text/html"
    assert r.headers["content-length"] == "156"


def test_404():
    r = requests.get("http://127.0.0.1:8080/not-exists.html")
    assert r.status_code == 404, r.status_code


def test_501():
    r = requests.get("http://127.0.0.1:8080/no-extension")
    assert r.status_code == 501, r.status_code


def test_image():
    r = requests.get("http://127.0.0.1:8080/cat.jpg")
    assert r.status_code == 200, r.status_code
    assert r.headers["content-type"] == "image/jpeg"


def test_no_root():
    r = requests.get("http://127.0.0.1:8080")
    assert r.status_code == 501, r.status_code


def test_bad_method():
    r = requests.put("http://127.0.0.1:8080", data={"no": "data"})
    assert r.status_code == 405, r.status_code


def test_bad_path():
    r = requests.get("http://127.0.0.1:8080/%/.sa._$''")
    assert r.status_code == 404, r.status_code


if __name__ == '__main__':
    test_ideal_case()
    test_404()
    test_501()
    test_image()
    test_no_root()
    test_bad_method()
    test_bad_path()
