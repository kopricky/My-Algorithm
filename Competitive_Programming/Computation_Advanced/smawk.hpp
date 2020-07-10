#include "../header.hpp"

template<class Func> class SMAWK {
private:
    void reduce(const vector<int>& row, const vector<int>& column, vector<int>& st){
        int idy = 0;
        while(idy < (int)column.size()){
            if(st.empty()){
                st.push_back(column[idy++]);
            }else{
                while(true){
                    if(func(row[st.size() - 1], st.back()) >= func(row[st.size() - 1], column[idy])){
                        st.pop_back();
                        if(st.empty()) break;
                    }else{
                        if(st.size() < row.size()) st.push_back(column[idy]);
                        ++idy;
                        break;
                    }
                }
            }
        }
    }
    template<typename T>
    void interpolate(const vector<int>& row, const vector<int>& column,
                                    const vector<pair<T, int> >& even_ans, vector<pair<T, int> >& ans){
        int st = 0;
        for(int i = 0; i < (int)row.size(); i += 2){
            const int ed = ((i < (int)row.size() - 1) ? even_ans[i / 2].second : column.back());
            T min_e = numeric_limits<T>::max();
            int min_y = -1;
            for(; st < (int)column.size() && column[st] <= ed; ++st){
                const T res = func(row[i], column[st]);
                if(min_e > res) min_e = res, min_y = column[st];
            }
            ans.emplace_back(min_e, min_y);
            if(i < (int)row.size() - 1) ans.push_back(even_ans[i / 2]);
            --st;
        }
    }
    template<typename T>
    void solve_impl(const vector<int>& row, const vector<int>& column, vector<pair<T, int> >& ans){
        if((int)row.size() == 1){
            T min_e = numeric_limits<T>::max();
            int min_y = -1;
            for(const int y : column){
                const T res = func(row[0], y);
                if(min_e > res) min_e = res, min_y = y;
            }
            ans.emplace_back(min_e, min_y);
            return;
        }
        vector<int> even_row;
        vector<pair<T, int> > even_ans;
        for(int i = 1; i < (int)row.size(); i += 2) even_row.push_back(row[i]);
        if(row.size() < column.size()){
            vector<int> new_column;
            reduce(row, column, new_column);
            solve_impl(even_row, new_column, even_ans);
            interpolate(row, new_column, even_ans, ans);
        }else{
            solve_impl(even_row, column, even_ans);
            interpolate(row, column, even_ans, ans);
        }
    }

private:
    const Func& func;

public:
    SMAWK(const Func& _func) : func(_func){}
    // クエリの対象となる row, column
    // ans: (min, arg_min)
    template<typename T>
    void solve(const vector<int>& row, const vector<int>& column, vector<pair<T, int> >& ans){
        if(row.empty() || column.empty()) return;
        solve_impl(row, column, ans);
    }
};

// examle(コスト関数を明示的に行列で与えている)
template<typename T> class Func {
public:
    const vector<vector<T> > A;
    Func(const vector<vector<T> >& _A) : A(_A){}
    T operator()(const int x, const int y) const {
        return A[x][y];
    }
};