#include "header.hpp"

double f(double t, double y)
{
    return y * cos(t);
}

pair<double, double> proceed(double t, double y, double h)
{
    double k1 = f(t, y);
    double k2 = f(t + h / 2, y + h / 2 * k1);
    double k3 = f(t + h / 2, y + h / 2 * k2);
    double k4 = f(t + h, y + h * k3);
    return make_pair(t + h, y + h * (k1 + 2 * (k2 + k3) + k4) / 6);
}

vector<pair<double, double> > runge_kutta(double t0, double y0, double h, double n)
{
    vector<pair<double, double> > result(n + 1);
    result[0] = {t0, y0};
    for(int i = 1; i <= n; ++i){
        result[i] = proceed(result[i-1].first, result[i-1].second, h);
    }
    return result;
}
