#include <iostream>
#include <array>
#include <cmath>

using namespace std;

const int sz = 2;
const int cnt_limit = 10000;
const double dt = 5;

array<double, sz> operator+(const array<double, sz>& arg1, const array<double, sz>& arg2)
{
    array<double, sz> res;
    for(int i = 0; i < sz; i++){
        res[i] = arg1[i] + arg2[i];
    }
    return res;
}

array<double, sz> operator*(const array<double, sz>& arg1, const double arg2)
{
    array<double, sz> res;
    for(int i = 0; i < sz; i++){
        res[i] = arg1[i] * arg2;
    }
    return res;
}

array<double, sz> operator/(const array<double, sz>& arg1, const double arg2)
{
    array<double, sz> res;
    for(int i = 0; i < sz; i++){
        res[i] = arg1[i] / arg2;
    }
    return res;
}


void df(array<double, sz>& x, array<double, sz>& res)
{
    res[0] = -pow(x[0],3)-pow(x[1],3),res[1] = x[0]-pow(x[1],5);
}

void runge_kutta(array<double, sz>& st_pos, array<double, sz>& ed_pos)
{
    array<array<double, sz>, 5> tilt;
    df(st_pos, tilt[0]);
    array<double, sz> nx_pos1 = st_pos + tilt[0] * (0.5*dt);
    df(nx_pos1, tilt[1]);
    array<double, sz> nx_pos2 = st_pos + tilt[1] * (0.5*dt);
    df(nx_pos2, tilt[2]);
    array<double, sz> nx_pos3 = st_pos + tilt[2] * dt;
    df(nx_pos3, tilt[3]);
    tilt[4] = (tilt[0] + tilt[1]*2.0 + tilt[2]*2.0 + tilt[3]) / 6.0;
    ed_pos = st_pos + tilt[4] * dt;
}

int main()
{
    array<double, sz> input;
    for(int i = 0; i < sz; i++){
        cin >> input[i];
    }
    int cnt = 0;
    for(int i = 0; i <= cnt_limit; i++){
        array<double, sz> output;
        cout << input[0] << " " << input[1] << "\n";
        runge_kutta(input, output);
        input = output;
    }
}
