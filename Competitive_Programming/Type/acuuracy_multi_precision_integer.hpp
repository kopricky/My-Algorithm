#include "../header.hpp"

template<int acc> class AMPI : public deque<int> {
private:

    static constexpr int root = 5;
    static constexpr int MOD_ = 924844033;

    static void trim_digit(AMPI& num){
        while((int)num.size() >= 1 && num.back() == 0) num.pop_back();
        if((int)num.size() == 1 && num[0] == 0){ num.zero = true; return; }
		while((int)num.size() < acc) num.push_front(0), num.ex--;
		while((int)num.size() > acc + 1) num.pop_front(), num.ex++;
		rounding(num);
    }
	static void rounding(AMPI& num){
		if((int)num.size() != acc + 1) return;
		if(num[0] >= 5){
			int pos = 1;
			do{ num[pos]++;
				if(num[pos] != 10) break;
				num[pos] = 0;
			}while(++pos <= acc);
			if(pos == acc+1) num.push_back(1), num.pop_back(), num.ex++;
		}
		num.pop_front(), num.ex++;
	}
    static bool abs_less(const AMPI& a, const AMPI& b){
        if(a.ex != b.ex) return a.ex < b.ex;
        for(int index = acc - 1; index >= 0; index--){
            if(a[index] != b[index]) return a[index] < b[index];
        }
        return false;
    }
    static void num_sbst(AMPI& a, const AMPI& b){
        a.resize(acc), a.zero = false, a.ex = b.ex;
        for(int i = 0; i < acc; i++) a[i] = b[i];
    }
    static void add(const AMPI& a, const AMPI& b, AMPI& res){
        int diff = a.ex - b.ex, carry = 0;
        if(abs(diff) > acc) return (diff > 0) ? num_sbst(res, a) : num_sbst(res, b);
		if(diff >= 0){
			num_sbst(res, a);
            if(diff) res.push_front(0), res.ex--;
			for(int i = !diff; i <= acc; i++){
	            int val = res[i-!diff] + (i <= acc-diff ? b[i+diff-1] : 0) + carry;
	            carry = val/10;
	            res[i-!diff] = val%10;
	        }
			if(carry) res.push_back(1);
		}else{
            num_sbst(res, b);
            res.push_front(0), res.ex--;
			for(int i = 0; i <= acc; i++){
	            int val = res[i] + (i <= acc+diff ? a[i-diff-1] : 0) + carry;
	            carry = val/10;
	            res[i] = val%10;
	        }
			if(carry) res.push_back(1);
		}
        trim_digit(res);
    }
    static void sub(const AMPI& a, const AMPI& b, AMPI& res){
        int diff = a.ex - b.ex, carry = 0;
        num_sbst(res, a);
        if(diff > acc) return;
		if(diff){
			res.push_front(0), res.ex--;
			int carry = 0;
			for(int i = 0; i <= acc; i++){
				int val = res[i] - carry - (i <= acc-diff ? b[i+diff-1] : 0);
				if(val < 0){
					carry = 1, val += 10;
				}else{
					carry = 0;
				}
				res[i] = val;
			}
		}else{
            for(int i = 0; i < acc; i++){
				int val = res[i] - carry - b[i];
				if(val < 0){
					carry = 1, val += 10;
				}else{
					carry = 0;
				}
				res[i] = val;
			}
		}
        trim_digit(res);
    }
    static int add_(const int x, const int y) { return (x + y < MOD_) ? x + y : x + y - MOD_; }
    static int mul_(const int x, const int y) { return (long long)x * y % MOD_; }
    static int power(int x, int n){
        int res = 1;
        while(n > 0){
            if(n & 1) res = mul_(res, x);
            x = mul_(x, x);
            n >>= 1;
        }
        return res;
    }
    static int inverse(const int x) { return power(x, MOD_ - 2); }
    static void ntt(vector<int>& a, const bool rev = false){
        int i,j,k,s,t,v,w,wn;
        const int size = (int)a.size();
        const int height = (int)log2(2 * size - 1);
        for(i = 0; i < size; i++){
            j = 0;
            for(k = 0; k < height; k++) j |= (i >> k & 1) << (height - 1 - k);
            if(i < j) std::swap(a[i], a[j]);
        }
        for(i = 1; i < size; i *= 2) {
            w = power(root, (MOD_ - 1) / (i * 2));
            if(rev) w = inverse(w);
            for(j = 0; j < size; j += i * 2){
                wn = 1;
                for(k = 0; k < i; k++){
                    s = a[j + k], t = mul_(a[j + k + i], wn);
                    a[j + k] = add_(s, t);
                    a[j + k + i] = add_(s, MOD_ - t);
                    wn = mul_(wn, w);
                }
            }
        }
        if(rev){
            v = inverse(size);
            for (i = 0; i < size; i++) a[i] = mul_(a[i], v);
        }
    }
    static void mul(const AMPI& a, const AMPI& b, AMPI& res){
        const int size = (int)a.size() + (int)b.size() - 1;
        int t = 1;
        while (t < size) t <<= 1;
        vector<int> A(t, 0), B(t, 0);
        for(int i = 0; i < (int)a.size(); i++) A[i] = a[i];
        for(int i = 0; i < (int)b.size(); i++) B[i] = b[i];
        ntt(A), ntt(B);
        for(int i = 0; i < t; i++) A[i] = mul_(A[i], B[i]);
        ntt(A, true);
        res.resize(size);
        int carry = 0;
        for(int i = 0; i < size; i++){
            int val = A[i] + carry;
            carry = val / 10;
            res[i] = val % 10;
        }
        if(carry) res.push_back(carry);
        trim_digit(res);
    }

    class MPI : public deque<int> {
    public:
        MPI(){ push_back(0); }
        inline static void trim_digit(MPI& num){
            while(num.back() == 0 && (int)num.size() >= 2) num.pop_back();
        }
        bool isZero() const {
            return (int)size() == 1 && (*this)[0] == 0;
        }
        static void add(const MPI& a, const MPI& b, MPI& res){
            res = a;
            int mx = (int)max(a.size(), b.size());
            res.resize(mx, 0);
            int carry = 0;
            for(int i = 0; i < mx; i++){
                int val = res[i] + ((i < (int)b.size()) ? b[i] : 0) + carry;
                carry = val/10;
                res[i] = val%10;
            }
            if(carry) res.push_back(1);
        }
        static void sub(const MPI& a, const MPI& b, MPI& res){
            res = a;
            int carry = 0;
            for(int i = 0; i < (int)res.size(); i++){
                int val = res[i] - carry - ((i < (int)b.size()) ? b[i] : 0);
                if(val < 0){
                    carry = 1, val += 10;
                }else{
                    carry = 0;
                }
                res[i] = val;
            }
            trim_digit(res);
        }
        bool operator<(const MPI& another) const {
            if(size() != another.size()) return size() < another.size();
            for(int index = (int)size() - 1; index >= 0; index--){
                if((*this)[index] != another[index]) return (*this)[index] < another[index];
            }
            return false;
        }
        static bool div_geq(const MPI& mod, const MPI& num){
            if((int)mod.size() != (int)num.size()) return (int)mod.size() > (int)num.size();
            int n = (int)mod.size();
            for(int i = 0; i < n; i++){
                if(mod[n-1-i] != num[n-1-i]){
                    return mod[n-1-i] > num[n-1-i];
                }
            }
            return true;
        }
        static void div_(const MPI& a, const MPI& b, MPI& res){
            vector<MPI> mult(9);
            MPI mod;
            mult[0] = b;
            for(int i = 0; i < 8; i++) add(mult[i], b, mult[i+1]);
            for(int i = (int)a.size() - 1; i >= 0; i--){
                if(mod.isZero()){
                    mod.back() = a[i];
                }else{
                    mod.push_front(a[i]);
                }
                if(div_geq(mod, b)){
                    int l = 0, r = 9;
                    while(r-l>1){
                        int mid = (l+r)/2;
                        if(mod < mult[mid]){
                            r = mid;
                        }else{
                            l = mid;
                        }
                    }
                    MPI mod_ = mod;
                    sub(mod_, mult[l], mod);
                    res.push_front(l+1);
                }else{
                    res.push_front(0);
                }
            }
            trim_digit(res);
        }
    };

    static void mpi_AMPI(MPI& a, const AMPI& b){
        if(b.zero){ a = MPI(0); return; }
        int n = (int)b.size();
        a.resize(n);
        for(int i = 0; i < n; i++) a[i] = b[i];
    }

    static void AMPI_mpi(AMPI& a, const MPI& b){
        if(b.isZero()){ a = AMPI(); return; }
        int n = (int)b.size();
        a.resize(n);
        for(int i = 0; i < n; i++) a[i] = b[i];
    }


public:

	friend ostream& operator<<(ostream& os, const AMPI& num) {
        if(num.zero){ os << "0."; for(int i = 0; i < acc-1; i++) os << '0';
                    os << "+e0"; return os; }
		if(num.sign) os << '-';
		os << num.back() << '.';
		for(int i = 0; i < acc-1; i++) os << num[acc-2-i];
		os << 'e';
        if(num.ex+acc-1 >= 0) os << '+';
        os << num.ex+acc-1;
        return os;
	}

    friend istream& operator>>(istream& is, AMPI& num) {
        string s;
        is >> s;
        num = AMPI(s);
        return is;
    }

    void print_decimal(int decimal) const {
        if(zero){ cout << "0."; for(int i = 0; i < decimal; i++) cout << '0';}
        if(sign) cout << '-';
        for(int i = max(ex+acc-1,0LL); i >= -decimal; i--){
            cout << ((i-ex >= 0 || i-ex >= acc)?(*this)[i-ex]:'0');
            if(i == 0) cout << '.';
        }
    }

    double to_double() const {
        if(zero){ return 0.0; }
        double res = 0.0, d = 1.0;
        for(int i = 0; i < acc; i++){
            res += (*this)[i] * d, d *= 10.0;
        }
        return res * pow(10.0, ex);
    }

    AMPI& operator=(long long num) {
        *this = AMPI(num);
        return *this;
    }

    bool operator<(const AMPI& num) const {
        if(zero) return !num.zero && !num.sign;
        if(num.zero) return sign;
        if(sign ^ num.sign) return sign;
        if(ex != num.ex) return (ex < num.ex) ^ sign;
        for(int index = acc - 1; index >= 0; index--){
            if((*this)[index] != num[index]) return ((*this)[index] < num[index]) ^ sign;
        }
        return false;
    }

    bool operator<(const long long num) const {
        return *this < AMPI(num);
    }

    friend bool operator<(const long long num, const AMPI& another){
        return AMPI(num) < another;
    }

    bool operator>(const AMPI& num) const {
        return num < *this;
    }

    bool operator>(const long long num) const {
        return *this > AMPI(num);
    }

    friend bool operator>(const long long num, const AMPI& another){
        return AMPI(num) > another;
    }

    bool operator<=(const AMPI& num) const {
        return !(*this > num);
    }

    bool operator<=(const long long num) const {
        return *this <= AMPI(num);
    }

    friend bool operator<=(const long long num, const AMPI& another){
        return AMPI(num) <= another;
    }

    bool operator>=(const AMPI& num) const {
        return !(*this < num);
    }

    bool operator>=(const long long num) const {
        return *this >= AMPI(num);
    }

    friend bool operator>=(const long long num, const AMPI& another){
        return AMPI(num) >= another;
    }

    bool operator==(const AMPI& num) const {
        if(zero || num.zero) return zero && num.zero;
        if(sign ^ num.sign) return false;
        if(ex != num.ex) return false;
        for(int index = acc - 1; index >= 0; index--){
            if((*this)[index] != num[index]) return false;
        }
        return true;
    }

    bool operator==(const long long num) const {
        return *this == AMPI(num);
    }

    friend bool operator==(const long long num, const AMPI& another){
        return AMPI(num) == another;
    }

    bool operator!=(const AMPI& num) const {
        return !(*this == num);
    }

    bool operator!=(const long long num) const {
        return *this != AMPI(num);
    }

    friend bool operator!=(const long long num, const AMPI& another){
        return AMPI(num) != another;
    }

    explicit operator bool() const noexcept { return !zero; }
    bool operator!() const noexcept { return !static_cast<bool>(*this); }

    explicit operator int() const noexcept { return (int)this->to_ll(); }
    explicit operator long long() const noexcept { return this->to_ll(); }

    AMPI operator+() const {
        return *this;
    }

    AMPI operator-() const {
        AMPI res = *this;
        res.sign = !sign;
        return res;
    }

    friend AMPI abs(const AMPI& num){
        AMPI res = num;
        res.sign = false;
        return res;
    }

    AMPI operator+(const AMPI& num) const {
        if(zero){ AMPI res = num; return res; }
        if(num.zero){ AMPI res = *this; return res; }
        AMPI res; res.sign = sign;
        if(sign != num.sign){
            if(abs_less(*this, num)){
                res.sign = num.sign;
                sub(num, *this, res);
                return res;
            }else{
                sub(*this, num, res);
                return res;
            }
        }
        add(*this, num, res);
        return res;
    }

    AMPI operator+(long long num) const {
        return *this + AMPI(num);
    }

    friend AMPI operator+(long long a, const AMPI& b){
        return b + a;
    }

    AMPI& operator+=(const AMPI& num){
        *this = *this + num;
        return *this;
    }

    AMPI& operator+=(long long num){
        *this = *this + num;
        return *this;
    }

    AMPI& operator++(){
        return *this += 1;
    }

    AMPI operator++(int){
        AMPI res = *this;
        *this += 1;
        return res;
    }

    AMPI operator-(const AMPI& num) const {
        if(zero){ AMPI res = num; res.sign = !res.sign; return res; }
        if(num.zero){ AMPI res = *this; return res; }
        if(sign != num.sign){
            AMPI res; res.sign = sign;
            add(*this, num, res);
            return res;
        }
        AMPI res; res.sign = (abs_less(*this, num) ^ sign);
        if(res.sign){
            sub(num, *this, res);
        }else{
            sub(*this, num, res);
        }
        return res;
    }

    AMPI operator-(long long num) const {
        return *this - AMPI(num);
    }

    friend AMPI operator-(long long a, const AMPI& b){
        return AMPI(a) - b;
    }

    AMPI& operator-=(const AMPI& num){
        *this = *this - num;
        return *this;
    }

    AMPI& operator-=(long long num){
        *this = *this - num;
        return *this;
    }

    AMPI& operator--(){
        return *this -= 1;
    }

    AMPI operator--(int){
        AMPI res = *this;
        *this -= 1;
        return res;
    }

    AMPI operator*(const AMPI& num) const {
        if(zero || num.zero) return AMPI();
        AMPI res; res.zero = false; res.sign = (sign^num.sign);
        res.ex = ex + num.ex;
        mul(*this, num, res);
        return res;
    }

    AMPI operator*(long long num) const {
        return *this * AMPI(num);
    }

    friend AMPI operator*(long long a, const AMPI& b){
        return b * a;
    }

    AMPI& operator*=(const AMPI& num){
        *this = *this * num;
        return *this;
    }

    AMPI& operator*=(long long num){
        *this = *this * num;
        return *this;
    }

    AMPI operator/(const AMPI& num) const {
        assert(!num.zero);
        if(zero){ return AMPI(); }
        MPI kp, res_, num_;
        mpi_AMPI(num_, num);
        AMPI res; res.zero = false; res.sign = (sign^num.sign); res.ex = ex - num.ex;
        mpi_AMPI(kp, *this);
        if(abs_less(*this, num)) kp.push_front(0), res.ex--;
        for(int i = 0; i < acc; i++) kp.push_front(0), res.ex--;
        MPI::div_(kp, num_, res_);
        AMPI_mpi(res, res_);
        trim_digit(res);
        return res;
    }

    AMPI operator/(long long num) const {
        return *this / AMPI(num);
    }

    friend AMPI operator/(long long a, const AMPI& b){
        return AMPI(a) / b;
    }

    AMPI& operator/=(const AMPI& num){
        *this = *this / num;
        return *this;
    }

    AMPI& operator/=(long long num){
        *this = *this / num;
        return *this;
    }

    AMPI div2() const {
        if(zero) return AMPI();
        AMPI res = *this;
        int carry = 0;
        for(int i = acc-1; i >= 0; i--){
            int val = (*this)[i]+carry*10;
            carry = val%2;
            if(i != acc-1 || val >= 2){
                res[i] = val/2;
            }else{
                res[i] = 0;
            }
        }
        if(carry) res.push_front(5), res.ex--;
        trim_digit(res);
        return res;
    }

    friend AMPI sqrt(AMPI x){
        if(x <= AMPI(0)) return AMPI();
        AMPI s = 1, t = x;
        while(s < t){
            s = s + s, t = t.div2();
        }
        do{ t = s, s = (x / s + s).div2();
        }while(s < t);
        trim_digit(t);
        return t;
    }

    friend AMPI log10(const AMPI& x){
        assert(x > AMPI(0));
        return AMPI(acc + x.ex);
    }

    friend AMPI pow(AMPI a, long long b){
        if(a.zero) return AMPI();
        assert(b >= 0);
        AMPI res(1);
        while(b){
            if(b % 2) res *= a;
            a *= a;
            b = b/2;
        }
        return res;
    }

    bool sign, zero;
	long long ex;

    AMPI() : zero(true){}

    AMPI(long long val) : sign(false), zero(false), ex(0){
        if(val == 0){ zero = true; return; }
        if(val < 0) sign = true, val = -val;
        while(val) push_back(val%10), val /= 10;
		trim_digit(*this);
    }

    AMPI(const string& s) : sign(false), zero(false), ex(0){
        if(s.empty() || s == "0"){ zero = true; return; }
        if(s[0] == '-') sign = true;
        for(int i = (int)s.size() - 1; i >= sign; i--){
            if(s[i] == '.'){
                ex = i + 1 - (int)s.size();
            }else{
                push_back(s[i]-'0');
            }
        }
		trim_digit(*this);
    }
};
