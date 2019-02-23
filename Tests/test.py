import subprocess
import urllib.parse
import urllib.request
import urllib.error
import json
import sys


def main():
    args = sys.argv
    code_path, problem_url = args[1], args[2]
    abs_path = '/home/meip-users/my_algorithm/Tests'
    exec_check = subprocess.call('g++ -std=c++14 -o {}/exec ./{}'.format(abs_path, code_path).split())
    if exec_check == 1:
        print('コンパイル失敗')
        return
    else:
        print('コンパイル成功')
    parsed_url = urllib.parse.urlparse(problem_url)
    problem_id = parsed_url.query[3:]
    url = 'https://judgedat.u-aizu.ac.jp/testcases/{}/header'.format(problem_id)
    user_agent = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.87 Safari/537.36'
    req = urllib.request.Request(url, method='GET', headers={'User-Agent':user_agent})
    body = json.load(urllib.request.urlopen(req))
    for header in body['headers']:
        for type in ['in', 'out']:
            testcase_url = 'https://judgedat.u-aizu.ac.jp/testcases/{}/{}/{}'.format(problem_id, header['serial'], type)
            subprocess.call('wget {} -q -O {}/test_case/{}.{}'.format(testcase_url, abs_path, header['serial'], type).split())
        input_file = open('{}/test_case/{}.in'.format(abs_path, header['serial']), mode='r')
        output_file = open('{}/test_case/{}.txt'.format(abs_path, header['serial']), mode='w')
        subprocess.run('{}/exec'.format(abs_path), stdin=input_file, stdout=output_file, shell=True)
        dif_res = subprocess.check_output('diff {}/test_case/{}.txt {}/test_case/{}.out'.format(abs_path, header['serial'], abs_path, header['serial']).split())
        if dif_res == b'':
            print('Testcase {}: AC'.format(header['serial']))
        else:
            print('Testcase {}: WA'.format(header['serial']))
            break

if __name__ == '__main__':
    main()