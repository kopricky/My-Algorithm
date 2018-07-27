// 四則演算の構文解析
#include <bits/stdc++.h>

using namespace std;

// <expr>   ::= <term> [ ('+'|'-') <term> ]*
// <term>   ::= <factor> [ ('*'|'/') <factor> ]*
// <factor> ::= <number> | '(' <expr> ')'
// <number> :== 非負の数

int expr(int& pos);
int term(int& pos);
int factor(int& pos);
int number(int& pos);

string s;

int expr(int& pos){
    int res = term(pos);
    while(s[pos] == '+' || s[pos] == '-'){
        if(s[pos] == '+'){
            res += term(++pos);
        }else{
            res -= term(++pos);
        }
    }
    return res;
}

int term(int& pos){
    int res = factor(pos);
    while(s[pos] == '*' || s[pos] == '/'){
        if(s[pos] == '*'){
            res *= factor(++pos);
        }else{
            res /= factor(++pos);
        }
    }
    return res;
}

int factor(int& pos){
    if(isdigit(s[pos])) return number(pos);
    pos++;  //'('をとばす
    int res = expr(pos);
    pos++;  //')'をとばす
    return res;
}

int number(int& pos){
    string res;
    while(isdigit(s[pos])){
        res.push_back(s[pos++]);
    }
    return stoi(res);
}

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    cin >> s;
    int pos = 0;
    cout << expr(pos) << "\n";
    return 0;
}
