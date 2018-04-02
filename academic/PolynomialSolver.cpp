//多項式の求根アルゴリズム(Laguerre's method)
class PolynomialSolver
{
private:
    using C = complex<double>;
    const double EPS = 1e-10;
    pair<vector<C>, C> horner(const vector<C>& a, const C x0){
        int n = (int)a.size();
        vector<C> b = vector<C>(max(1, n-1));
        for(int i = n-1; i > 0; i--){
            b[i-1] = a[i] + (i < n-1 ? b[i] * x0 : 0);
        }
        return make_pair(b, a[0] + b[0] * x0);
    }
    C eval(const vector<C>& p, const C x){
        return horner(p, x).second;
    }
    vector<C> derivative(const vector<C>& p){
        int n = (int)p.size();
        vector<C> r = vector<C>(max(1, n-1));
        for(int i = 1; i < n; i++){
            r[i-1] = p[i] * C(i);
        }
        return r;
    }
    int cmp(const C x, const C y){
        double diff = abs(x) - abs(y);
        return diff < -EPS ? -1 : (diff > EPS ? 1 : 0);
    }
    C FindOneRoot(const vector<C>& p, C x){
        int n = (int)p.size() - 1;
        vector<C> p1 = derivative(p);
        vector<C> p2 = derivative(p1);
        for(int step = 0; step < 10000; step++){
            C y = eval(p,x);
            if(cmp(y,0) == 0) break;
            C G = eval(p1,x) / y;
            C H = G*G - eval(p2,x) - y;
            C R = sqrt(C(n-1) * (H * C(n) - G * G));
            C D1 = G + R, D2 = G - R;
            C a = C(n) / (cmp(D1, D2) > 0 ? D1 : D2);
            x -= a;
            if(cmp(a,0) == 0) break;
        }
        return x;
    }
public:
    vector<C> res;
    //引数はp[0]+p[1]x+p[2]x^2+...+p[k]x^kの形
    PolynomialSolver(const vector<C>& p){
        vector<C> q = p;
        while((int)q.size() > 2){
            C z(rand() / double(RAND_MAX), rand() / double(RAND_MAX));
            z = FindOneRoot(q,z);
            z = FindOneRoot(p,z);
            q = horner(q,z).first;
            res.push_back(z);
        }
        res.push_back(-q[0]/q[1]);
    }
};
