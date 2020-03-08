#include "../header.hpp"

// 四則演算の構文解析
template<typename T> class Parser {
private:
    T expr(const string& s, int& pos){
        T res = term(s, pos);
        while(s[pos] == '+' || s[pos] == '-'){
            if(s[pos] == '+'){
                res += term(s, ++pos);
            }else{
                res -= term(s, ++pos);
            }
        }
        return res;
    }
    T term(const string& s, int& pos){
        T res = factor(s, pos);
        while(s[pos] == '*' || s[pos] == '/'){
            if(s[pos] == '*'){
                res *= factor(s, ++pos);
            }else{
                res /= factor(s, ++pos);
            }
        }
        return res;
    }
    T factor(const string& s, int& pos){
        if(isdigit(s[pos])) return number(s, pos);
        pos++; // '('をとばす
        T res = expr(s, pos);
        pos++; // ')'をとばす
        return res;
    }
    T number(const string& s, int& pos){
        T res = 0;
        do{
            res *= 10;
            res += (s[pos++] - '0');
        }while(isdigit(s[pos]));
        return res;
    }
 
public:
    Parser(){}
    T solve(const string& s){
        int pos = 0;
        return expr(s, pos);
    }
};