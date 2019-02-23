import subprocess
import urllib.parse
import urllib.request
import urllib.error
import json
import requests

def main():
    problem_url = input()
    parsed_url = urllib.parse.urlparse(problem_url)
    problem_number = parsed_url.query[3:]
    url = 'https://judgedat.u-aizu.ac.jp/testcases/{}/header'.format(problem_number)
    req = urllib.request.Request(url)
    try:
        with urllib.request.urlopen(req) as res:
            body = json.load(urllib.request.urlopen(req))
    except urllib.error.HTTPError as err:
        print(err.code)
    except urllib.error.URLError as err:
        print(err.reason)
    for header in body['headers']:
        for type in ['in', 'out']:
            testcase_url = 'https://judgedat.u-aizu.ac.jp/testcases/{}/{}/{}'.format(problem_number, header['serial'], type)
            subprocess.call('wget {} -O ./test_case/{}.{}'.format(testcase_url, header['serial'], type).split())
        if subprocess.check_output()

if __name__ == '__main__':
    main()