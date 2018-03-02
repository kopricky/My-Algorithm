// max c * x s.t. A*x <= b, x >= 0
class Simplex {
private:
    using Arr = vector<double>;
    using Mat = vector<vector<double> >;
    bool LT(double a, double y){
        return y-a > EPS;
    }
    bool EQ(double a, double y){
        return abs(a-y) < EPS;
    }
    bool LE(double a, double y){
        return y-a > -EPS;
    }
    vector<int> index;
    Mat a;
    int row,column,L;

    void Set(const Mat& A, const Arr& b, const Arr& c){
        infinity = none = false;
        row = A.size(),column = A[0].size() + 1;
        index.resize(row+column);
    	rep(i,row+column){
            index[i] = i;
        }
        L = row;
        a.resize(row+2,vector<double>(column+1,0));
        rep(i,row){
    	    rep(j,column-1){
                a[i][j] = -A[i][j];
            }
    	    a[i][column-1] = 1;
    	    a[i][column] = b[i];
    	    if(a[L][column] > a[i][column]){
                L = i;
            }
    	}
    	rep(j,column-1){
            a[row][j] = c[j];
        }
    	a[row+1][column-1] = -1;
    }

    void solve(Mat& a){
        for(int E = column-1;;){
    	    if(L < row){
                swap(index[E],index[L+column]);
        		a[L][E] = 1 / a[L][E];
                rep(j,column+1){
                    if(j != E){
                        a[L][j] *= -a[L][E];
                    }
                }
        		rep(i,row+2){
                    if(i != L){
                        rep(j,column+1){
                            if(j != E){
                                a[i][j] += a[i][E] * a[L][j];
                            }
                        }
            		    a[i][E] = a[i][E] * a[L][E];
                    }
        		}
    	    }
    	    E = -1;
    	    rep(j,column){
                if(E < 0 || index[E] > index[j]){
                    if(LT(0,a[row+1][j]) || (EQ(a[row+1][j],0) && LT(0,a[row][j]))){
                        E = j;
                    }
                }
            }
    	    if(E < 0) break;
            L = -1;
            rep(i,row){
                if(LT(a[i][E], 0)){
                    if(L < 0 || LE(a[L][column] / a[L][E], a[i][column] / a[i][E])){
                        L = i;
                    }
                }
            }
            if(L < 0){
                infinity = true;
                return;
            }
    	}
    	if(LT(a[row+1][column], 0)){
    	    none = true;
    	    return;
    	}
    	x.assign(column-1, 0);
    	rep(i,row){
            if(index[column+i] < column-1){
                x[index[column+i]] = a[i][column];
            }
        }
    	ans = a[row][column];
    }
public:
    bool infinity, none;
    double ans;
    Arr x;
    Simplex(const Mat& A, const Arr& b, const Arr& c){
        Set(A,b,c);
        solve(a);
    }
};
