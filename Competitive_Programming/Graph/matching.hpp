#include "../header.hpp"

#define EPS 1e-10

template<typename T> class mat : public vector<vector<T> > {
private:
    int r,c;    //行,列
public:
    inline int row() const {
        return r;
    }
    inline int column() const {
        return c;
    }
    mat(int n, int m, T val = 0){
        r = n, c = m;
        for(int i = 0; i < n; i++){
            this->push_back(vector<T>(m, val));
        }
    }
    int rank(){
        int res = 0;
        for(int i = 0; i < c; i++){
            if(res == r) return res;
            int pivot = res;
            for(int j = res + 1; j < r; j++){
                if(abs((*this)[j][i]) > abs((*this)[pivot][i])){
                    pivot = j;
                }
            }
            if(abs((*this)[pivot][i]) < EPS) continue;
            swap((*this)[pivot],(*this)[res]);
            for(int j = i + 1; j < c; j++){
                (*this)[res][j] /= (*this)[res][i];
            }
            for(int j = res + 1; j < r; j++){
                for(int k = i + 1; k < c; k++){
                    (*this)[j][k] -= (*this)[res][k]*(*this)[j][i];
                }
            }
            res++;
        }
        return res;
    }
    T det(){
        T ans = 1;
        mat B(r, r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                B[i][j] = (*this)[i][j];
            }
        }
        for(int i = 0; i < c; i++){
            int pivot = i;
            for(int j = i + 1; j < r; j++){
                if(abs(B[j][i]) > abs(B[pivot][i])){
                    pivot = j;
                }
            }
            if(abs(B[pivot][i]) < EPS) return (T)0;
            if(pivot != i) swap(B[i], B[pivot]), ans = -ans;
            ans *= B[i][i];
            for(int j = i + 1; j < r; j++){
                for(int k = c - 1; k >= i; k--){
                    B[j][k] -= B[i][k] * B[j][i] / B[i][i];
                }
            }
        }
        return ans;
    }
    inline void print() const {
        for(int i = 0; i < r; i++){
            for(int j = 0; j < (c-1); j++){
                cout << (*this)[i][j] << ",";
            }
            cout << (*this)[i][c-1] << endl;
        }
    }
};

class Matching {
private:
    int V;
    vector<pair<int, int> > es;
    vector<int> cnt;
    static constexpr int LOOP = 100;
    unsigned long rnd(){
        static unsigned long long x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned long long t = (x^(x<<11));
        x = y,y = z,z = w;
        return (w = (w^(w>>19))^(t^(t>>8)));
    }

public:
    Matching(int node_size) : V(node_size), cnt(V+1, 0){}
    void add_edge(int u, int v){
        if(u > v) swap(u, v);
        es.push_back(make_pair(u, v));
    }
    bool perfect_matchching(){
        for(int i = 0; i < LOOP; i++){
            mat<double> A(V, V);
            for(auto e : es){
                double r = (double)rnd()/((double)ULONG_MAX+1);
                A[e.first][e.second] = r, A[e.second][e.first] = -r;
            }
            if(abs(A.det()) > EPS) return true;
        }
        return false;
    }
    int maximal_matchching(){
        for(int i = 0; i < LOOP; i++){
            mat<double> A(V, V);
            for(auto e : es){
                double r = (double)rnd()/((double)ULONG_MAX+1);
                A[e.first][e.second] = r, A[e.second][e.first] = -r;
            }
            cnt[A.rank()]++;
        }
        return (int)(max_element(cnt.begin(), cnt.end()) - cnt.begin()) / 2;
    }
};