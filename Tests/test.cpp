#include <bits/stdc++.h>

using namespace std;

using int64 = long long;
const int mod = (int) (1e9 + 7);

const int64 infll = (1LL << 62) - 1;
const int inf = (1 << 30) - 1;

template< typename T1, typename T2 >
ostream &operator<<(ostream &os, const pair< T1, T2 > &p) {
  os << p.first << " " << p.second;
  return os;
}

template< typename T1, typename T2 >
istream &operator>>(istream &is, pair< T1, T2 > &p) {
  is >> p.first >> p.second;
  return is;
}

template< typename T >
ostream &operator<<(ostream &os, const vector< T > &v) {
  for(int i = 0; i < (int) v.size(); i++) {
    os << v[i] << (i + 1 != v.size() ? " " : "");
  }
  return os;
}

template< typename T >
istream &operator>>(istream &is, vector< T > &v) {
  for(T &in : v) is >> in;
  return is;
}

template< typename T1, typename T2 >
inline bool chmax(T1 &a, T2 b) { return a < b && (a = b, true); }

template< typename T1, typename T2 >
inline bool chmin(T1 &a, T2 b) { return a > b && (a = b, true); }

template< typename T = int64 >
vector< T > make_v(size_t a) {
  return vector< T >(a);
}

template< typename T, typename... Ts >
auto make_v(size_t a, Ts... ts) {
  return vector< decltype(make_v< T >(ts...)) >(a, make_v< T >(ts...));
}

template< typename T, typename V >
typename enable_if< is_class< T >::value == 0 >::type fill_v(T &t, const V &v) {
  t = v;
}

template< typename T, typename V >
typename enable_if< is_class< T >::value != 0 >::type fill_v(T &t, const V &v) {
  for(auto &e : t) fill_v(e, v);
}

template< typename F >
struct FixPoint : F {
  FixPoint(F &&f) : F(forward< F >(f)) {}

  template< typename... Args >
  decltype(auto) operator()(Args &&... args) const {
    return F::operator()(*this, forward< Args >(args)...);
  }
};

template< typename F >
inline decltype(auto) MFP(F &&f) {
  return FixPoint< F >{forward< F >(f)};
}


// NW
template< typename T, typename Compare = greater< T > >
vector< T > knapsack_limitations(const vector< int > &w, const vector< int > &m, const vector< T > &v,
                                 const int &W, const T &NG, const Compare &comp = Compare()) {
  const int N = (int) w.size();
  vector< T > dp(W + 1, NG), deqv(W + 1);
  dp[0] = T();
  vector< int > deq(W + 1);
  for(int i = 0; i < N; i++) {
    for(int a = 0; a < w[i]; a++) {
      int s = 0, t = 0;
      for(int j = 0; w[i] * j + a <= W; j++) {
        if(dp[w[i] * j + a] != NG) {
          auto val = dp[w[i] * j + a] - j * v[i];
          while(s < t && comp(val, deqv[t - 1])) --t;
          deq[t] = j;
          deqv[t++] = val;
        }
        if(s < t) {
          dp[j * w[i] + a] = deqv[s] + j * v[i];
          if(deq[s] == j - m[i]) ++s;
        }
      }
    }
  }
  return dp;
}

int cnt[55];

template< typename T >
T knapsack_limitations(const vector< T > &w, const vector< T > &m, const vector< int > &v, const T &W) {
  const int N = (int) w.size();
  auto v_max = *max_element(begin(v), end(v));
  if(v_max == 0) return 0;
  vector< int > ma(N);
  vector< T > mb(N);
//   for(int i = 0; i < N; i++) {
//     ma[i] = min< T >(m[i], v_max - 1);
//     mb[i] = m[i] - ma[i];
//   }
//   T sum = 0;
//   for(int i = 0; i < N; i++) sum += ma[i] * v[i];
//   T sum = 0;
//   for(int i = 0; i < N; i++) {
//     ma[i] = min< T >(m[i], max(0, v_max - 1 - cnt[v[i]]));
//     sum += ma[i] * v[i];
//     mb[i] = m[i] - ma[i];
//     cnt[v[i]] += ma[i];
//   }
// T sum = 0, ad = N;
// for(int i = 0; i < N; i++) {
//   ma[i] = min< T >({m[i], max(0, v_max - 1 - cnt[v[i]]), ad});
//   sum += ma[i] * v[i];
//   mb[i] = m[i] - ma[i];
//   cnt[v[i]] += ma[i];
//   ad -= ma[i];
// }
T sum = 0;
// int ad = N;
// for(int i = 0; i < N; i++) {
//   ma[i] = min< T >(m[i], max({v_max - 1, ad, 2}));
//   mb[i] = m[i] - ma[i];
//   ad -= ma[i];
//   sum += ma[i] * v[i];
// }
int ad = N;
for(int i = 0; i < N; i++) {
  ma[i] = min< T >(m[i], max(min(v_max - 1, ad), 2));
  mb[i] = m[i] - ma[i];
  ad -= ma[i];
  sum += ma[i] * v[i];
}
  auto dp = knapsack_limitations(v, ma, w, sum, T(-1), less<>());
  T ret = T();
  for(int i = 0; i < (int)dp.size(); i++) {
    if(dp[i] > W || dp[i] == -1) continue;
    T rest = W - dp[i], cost = i;
    for(int p = 0; p < N; ++p){
      auto get = min(mb[p], rest / w[p]);
      if(get == 0) break;
      cost += get * v[p];
      rest -= get * w[p];
    }
    ret = max(ret, cost);
  }
  return ret;
}


int main() {
  int N;
  int64 W;
  cin >> N >> W;
  vector< int > v(N);
  vector< int64 > w(N), m(N);
  for(int i = 0; i < N; i++) {
    cin >> v[i] >> w[i] >> m[i];
  }
  vector< int > ord(N);
  iota(begin(ord), end(ord), 0);
  sort(begin(ord), end(ord), [&](int a, int b) {
      return v[a] * w[b] > v[b] * w[a];
  });
  vector<int> nv(N);
  vector< int64 > nw(N), nm(N);
    for(int i = 0; i < N; ++i) nv[i] = v[ord[i]], nw[i] = w[ord[i]], nm[i] = m[ord[i]];
  cout << knapsack_limitations(nw, nm, nv, W) << endl;
}

