using LL = __int128;
istream& operator>>(istream& is, LL& v)
{
    string s;
    is >> s;
    v = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        if (isdigit(s[i])) { v = v * 10 + s[i] - '0'; }
    }
    if (s[0] == '-') { v *= -1; }
    return is;
}

ostream& operator<<(ostream& os, const LL& v)
{
    if (v == 0) { return (os << "0"); }
    LL num = v;
    if (v < 0) {
        os << '-';
        num = -num;
    }
    string s;
    for (; num > 0; num /= 10) { s.push_back((char)(num % 10) + '0'); }
    reverse(s.begin(), s.end());
    return (os << s);
}
