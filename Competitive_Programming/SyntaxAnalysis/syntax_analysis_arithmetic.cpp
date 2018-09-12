#include "../header.hpp"

// 四則演算の構文解析
template<typename T> class Parser
{
private:
    int pos;
    T expr(string& eq){
        T res = term();
        while(eq[pos] == '+' || eq[pos] == '-'){
            if(eq[pos++] == '+'){
                res += term();
            }else{
                res -= term();
            }
        }
        return res;
    }
    T term(string& eq){
        T res = factor();
        while(eq[pos] == '*' || eq[pos] == '/'){
            if(eq[pos++] == '*'){
                res *= term();
            }else{
                res /= term();
            }
        }
        return res;
    }
    T factor(string& eq){
        if(isdigit(eq[pos])) return number();
        pos++; // '('をとばす
        T res = expr();
        pos++; // ')'をとばす
        return res;
    }
    T number(string& eq){
        T res = 0;
        do{
            res *= 10;
            res += (eq[pos++] - '0');
        }while(isdigit(eq[pos]));
        return res;
    }

public:
    Parser(){}
    T solve(string& eq){
        pos = 0;
        return expr(eq);
    }
};
